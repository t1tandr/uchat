#include "uchat.h"

static gboolean filter(GtkListBoxRow* row, gpointer user_data) {
    UchatChatBox* chat = UCHAT_CHAT_BOX(gtk_list_box_row_get_child(row));
    const char* text = gtk_editable_get_text(GTK_EDITABLE(user_data));

    return strncmp(uchat_chat_box_get_name(chat), text, strlen(text)) == 0;
}

void chat_list_realize_cb(GtkListBox* self, gpointer user_data) {
    GObject* search_entry = gtk_builder_get_object(uchat->builder, "chat-search");

    gtk_list_box_set_filter_func(self, filter, search_entry, NULL);

    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();

    request = create_request(METHOD_GET, "/chats", data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "GET /chats");
    }

    response = g_async_queue_pop(uchat->responses);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 201) {
            cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");

            uchat->user->chats = get_chats_from_json_arr(response_data);

            t_list* list = uchat->user->chats;
            while (list) {
                UchatChatBox* box = uchat_chat_box_new((t_chat *)list->data);

                gtk_list_box_append(self, GTK_WIDGET(box));

                list = list->next;
            }
        }

        cJSON_Delete(response);
    }
    else {
        handle_error(RESPONSE_ERROR, "GET /chats");
    }
}


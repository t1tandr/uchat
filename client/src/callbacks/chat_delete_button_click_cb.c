#include "uchat.h"

void chat_delete_button_click_cb(GtkButton* self, gpointer user_data) {
    GtkWidget* dialog = GTK_WIDGET(gtk_builder_get_object(uchat->builder, "chat-new-dialog"));
    GtkEntry* chatname_entry = GTK_ENTRY(gtk_builder_get_object(uchat->builder, "chat-new-name-entry"));
    const char* chatname = gtk_editable_get_text(GTK_EDITABLE(chatname_entry));
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "name", chatname);

    request = create_request(METHOD_POST, "/chats", data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "POST /chats");
    }

    response = g_async_queue_pop(uchat->responses);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 201) {
        }

        cJSON_Delete(response);
    }
    else {
        handle_error(RESPONSE_ERROR, "POST /chats");
    }

    gtk_widget_hide(dialog);
}

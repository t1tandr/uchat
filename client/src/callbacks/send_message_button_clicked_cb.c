#include "uchat.h"

void send_message_button_clicked_cb(GtkButton* self, gpointer user_data) {
    GtkNotebook* chats = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    int id = gtk_notebook_get_current_page(chats);
    UchatMessageBox* chat = UCHAT_MESSAGE_BOX(gtk_notebook_get_nth_page(chats, id));

    gchar* text = uchat_message_box_get_text(chat);

    if (strlen(text) > 0) {
        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();
        cJSON_AddNumberToObject(data, "chat_id", uchat->user->current_chat->id);
        cJSON_AddStringToObject(data, "type", "text");
        cJSON_AddStringToObject(data, "content", text);

        request = create_request(METHOD_POST, "/messages", data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "POST /messages");
        }

        response = g_async_queue_pop(uchat->responses);
        
        if (cJSON_HasObjectItem(response, "status")) {
            status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
                handle_message_response(response_data);
            }

            cJSON_Delete(response);
        }
        else {
            handle_error(RESPONSE_ERROR, "POST /messages");
        }
    }

    free(text);
}


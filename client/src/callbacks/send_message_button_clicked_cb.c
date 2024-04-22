#include "uchat.h"

void send_message_button_clicked_cb(GtkButton* self, gpointer user_data) {
    GtkNotebook* chats = GTK_NOTEBOOK(gtk_builder_get_object(uchat->builder, "message-container"));
    int id = gtk_notebook_get_current_page(chats);
    UchatMessageBox* chat = UCHAT_MESSAGE_BOX(gtk_notebook_get_nth_page(chats, id));

    gchar* text = uchat_message_box_get_text(chat);

    if (strlen(mx_strtrim(text)) > 0) {
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

        response = send_request(uchat->servsock, request);

        if (response == NULL) {
            handle_error("uchat: error \'POST /messages\' request to server");
        }
        
        if (cJSON_HasObjectItem(response, "status")) {
            int status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 201) {
                cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
                t_message* message = get_message_from_json(response_data);

                uchat_message_box_add_message(chat, message, true);
            }

            cJSON_Delete(response);
        }
        else {
            handle_error("uchat: error \'POST /messages\' response from server");
        }
    }

    free(text);
}


#include "uchat.h"

void chat_box_realize_cb(UchatChatBox* self, gpointer user_data) {
    t_chat* chat = uchat_chat_box_get_chat(self);

    if (chat->messages == NULL) {
        cJSON* request = NULL;
        cJSON* response = NULL;
        cJSON* data = NULL;
        cJSON* headers = NULL;
        char route[128];

        sprintf(route, "/messages/%d", chat->id);

        headers = cJSON_CreateObject();
        cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

        data = cJSON_CreateObject();

        request = create_request(METHOD_GET, route, data, headers);

        int status = send_request(uchat->servsock, request);

        if (status != REQUEST_SUCCESS) {
            handle_error(REQUEST_ERROR, "GET /messages/{id}");
        }

        response = g_async_queue_pop(uchat->responses);

        if (response != NULL && cJSON_HasObjectItem(response, "status")) {
            status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

            if (status == 200) {
                cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");

                chat->messages = message_parse_from_json_arr(response_data);

                for (t_list* i = chat->messages; i != NULL; i = i->next) {
                    t_message* message = (t_message *)i->data;
                    if (i->next == NULL) {
                        chat->last_message = message;
                        uchat_chat_box_set_message(self, message);
                    }
                }
            }

            cJSON_Delete(response);
        }
        else {
            handle_error(RESPONSE_ERROR, "GET /messages/{id}");
        }
    }
}
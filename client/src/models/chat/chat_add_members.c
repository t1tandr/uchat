#include "uchat.h"

void chat_add_members(t_chat* chat) {
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;
    char route[128];

    sprintf(route, "/chat-members/%d", chat->id);

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();

    request = create_request(METHOD_GET, route, data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "GET /chat-members");
    }

    response = g_async_queue_pop(uchat->responses);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 200) {
            cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");

            chat->members = chat_member_parse_from_json_arr(response_data);
        }

        cJSON_Delete(response);
    }
    else {
        handle_error(RESPONSE_ERROR, "GET /chat-members");
    }
}


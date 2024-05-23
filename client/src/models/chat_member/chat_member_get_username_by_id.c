#include "uchat.h"

char* chat_member_get_username_by_id(int id) {
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;
    char route[128];

    sprintf(route, "/users/%d", id);

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();

    request = create_request(METHOD_GET, route, data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "GET /users/{id}");
    }

    response = g_async_queue_pop(uchat->responses);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 200) {
            cJSON* response_data = cJSON_GetObjectItemCaseSensitive(response, "data");
            char* username = cJSON_GetObjectItemCaseSensitive(response_data, "username")->valuestring;

            return username;
        }

        cJSON_Delete(response);

        return NULL;
    }
    else {
        handle_error(RESPONSE_ERROR, "GET /messages/{id}");
    }

    return NULL;
}



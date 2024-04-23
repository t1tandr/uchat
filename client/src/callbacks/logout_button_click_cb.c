#include "uchat.h"

void logout_button_click_cb(GtkWidget *self, gpointer user_data){
    cJSON* request = NULL;
    cJSON* response = NULL;
    cJSON* data = NULL;
    cJSON* headers = NULL;

    headers = cJSON_CreateObject();
    cJSON_AddStringToObject(headers, "Authorization", uchat->user->session);

    data = cJSON_CreateObject();

    request = create_request(METHOD_POST, "/logout", data, headers);

    int status = send_request(uchat->servsock, request);

    if (status != REQUEST_SUCCESS) {
        handle_error(REQUEST_ERROR, "POST /logout");
    }

    response = g_async_queue_pop(uchat->responses);

    if (response != NULL && cJSON_HasObjectItem(response, "status")) {
        status = cJSON_GetObjectItemCaseSensitive(response, "status")->valueint;

        if (status == 200) {
            remove("session.json");
            window_switch_child(uchat->builder, "homepage", "login-page");
        }

        cJSON_Delete(response);
    }
    else {
        handle_error(RESPONSE_ERROR, "POST /logouts");
    }
}


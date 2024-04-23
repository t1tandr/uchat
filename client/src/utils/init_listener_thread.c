#include "uchat.h"

static void handle_message_response(cJSON* data) {
    printf("new message!");
}

static void handle_chat_member_response(cJSON* data) {
    printf("new chat!");
}

static void handle_response(cJSON* response) {
    if (response == NULL
        || !cJSON_HasObjectItem(response, "status") 
        || !cJSON_HasObjectItem(response, "data")) {
            handle_error(RESPONSE_ERROR, "socket");
    }

    cJSON* data = cJSON_GetObjectItemCaseSensitive(response, "data");
    
    if (cJSON_GetObjectItemCaseSensitive(response, "type") && cJSON_GetObjectItemCaseSensitive(response, "content")) {
        handle_message_response(data);
    }
    else if (cJSON_GetObjectItemCaseSensitive(response, "role")) {
        handle_chat_member_response(data);
    }
}

void* listen_for_response(void* arg) {
    while (true) {
        cJSON* response = recv_response(uchat->servsock);

        if (response != NULL) {
            if (cJSON_HasObjectItem(response, "type")) {
                char* type = cJSON_GetObjectItemCaseSensitive(response, "type")->valuestring;

                if (strcmp(type, "regular") == 0) {
                    // mx_printstr(cJSON_Print(response));
                    // mx_printstr("\n\n");
                    g_async_queue_push(uchat->responses, response);
                }
                if (strcmp(type, "socket") == 0) {
                    mx_printstr(cJSON_Print(response));
                    mx_printstr("\n\n");
                }
            }
        }
    }

    pthread_exit(NULL);
}

void init_listener_thread() {
    int status = 0;
    pthread_t id;

    status = pthread_create(&id, NULL, listen_for_response, NULL);
    if (status != 0) {
        handle_error(PTHREAD_ERROR, strerror(errno));
    }

    status = pthread_detach(id);
    if (status != 0) {
        handle_error(PTHREAD_ERROR, strerror(errno));
    }
}


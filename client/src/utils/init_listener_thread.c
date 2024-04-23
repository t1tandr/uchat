#include "uchat.h"

void* listen_for_response(void* arg) {
    while (true) {
        cJSON* response = recv_response(uchat->servsock);

        if (response != NULL) {
            if (cJSON_HasObjectItem(response, "type")) {
                char* type = cJSON_GetObjectItemCaseSensitive(response, "type")->valuestring;

                if (strcmp(type, "regular") == 0) {
                    g_async_queue_push(uchat->responses, response);
                }
                if (strcmp(type, "socket") == 0) {
                    mx_printstr(cJSON_Print(response));
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


#include "server.h"

int check_update_message_dto(cJSON *message, int sock_fd) {
    if (message == NULL
        || !cJSON_HasObjectItem(message, "content")) {
        error_handler(sock_fd, "Invalid json", 400);
        return -1;
    }

    return 1;
}

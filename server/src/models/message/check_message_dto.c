#include "server.h"

int check_message_dto(cJSON *message, int sock_fd) {
    if (message == NULL
        || !cJSON_HasObjectItem(message, "chat_id")
        || !cJSON_HasObjectItem(message, "text")) {
        error_handler(sock_fd, "Invalid json", 400);
        return -1;
    }

    return 1;
}

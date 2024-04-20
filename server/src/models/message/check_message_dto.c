#include "server.h"

int check_message_dto(cJSON *message, int sock_fd) {
    if (message == NULL
        || !cJSON_HasObjectItem(message, "chat_id")
        || !cJSON_HasObjectItem(message, "type")
        || !cJSON_HasObjectItem(message, "content")) {
        error_handler(sock_fd, "Invalid json", 400);
        return -1;
    }

    char *type = cJSON_GetObjectItem(message, "type")->valuestring;

    if (strcmp(type, "text") == 0
        && strcmp(type, "photo") == 0
        && strcmp(type, "video") == 0
        && strcmp(type, "audio") == 0) {
            error_handler(sock_fd, "Undefined message type", 400);
            return -1;
        }

    return 1;
}

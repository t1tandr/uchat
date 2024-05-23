#include "server.h"

int check_chat_dto(cJSON *chat, int sock_fd) {
    if (chat == NULL
        || !cJSON_HasObjectItem(chat, "name")) {
        error_handler(sock_fd, "Invalid json", 400);
        return -1;
    }

    char *name = cJSON_GetObjectItem(chat, "name")->valuestring;

    if (strlen(name) > 32) {
        error_handler(sock_fd, "Invalid name", 400);
        return -1;
    }

    return 1;
}

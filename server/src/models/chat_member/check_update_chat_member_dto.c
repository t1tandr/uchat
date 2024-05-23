#include "server.h"

int check_update_chat_member_dto(cJSON *chat_member, int sock_fd) {
    if (chat_member == NULL
        || !cJSON_HasObjectItem(chat_member, "user_id")
        || !cJSON_HasObjectItem(chat_member, "role")) {
        error_handler(sock_fd, "Invalid json", 400);
        return -1;
    }

    char *role = cJSON_GetObjectItem(chat_member, "role")->valuestring;

    if (strcmp(role, "USER") != 0 && strcmp(role, "ADMIN") != 0) {
        error_handler(sock_fd, "Invalid role", 400);
        return -1;
    }

    return 1;
}

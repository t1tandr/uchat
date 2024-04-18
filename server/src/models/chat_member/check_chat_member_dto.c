#include "server.h"

int check_chat_member_dto(cJSON *chat_member, int sock_fd) {
    if (chat_member == NULL
        || !cJSON_HasObjectItem(chat_member, "chat_id")
        || !cJSON_HasObjectItem(chat_member, "user_id")) {
        error_handler(sock_fd, "Invalid json", 400);
        return -1;
    }

    return 1;
}

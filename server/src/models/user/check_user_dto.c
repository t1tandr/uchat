#include "server.h"

int check_user_dto(cJSON *user, int sock_fd) {
    if (user == NULL
        || !cJSON_HasObjectItem(user, "username")
        || !cJSON_HasObjectItem(user, "name")
        || !cJSON_HasObjectItem(user, "password")) {
        error_handler(sock_fd, "Invalid json", 400);
        return -1;
    }

    char *username = cJSON_GetObjectItem(user, "username")->valuestring;

    if (contains_space(username)) {
        error_handler(sock_fd, "Invalid username", 400);
        return -1;
    }

    return 1;
}

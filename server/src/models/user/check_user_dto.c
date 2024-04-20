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
    char *name = cJSON_GetObjectItem(user, "name")->valuestring;
    char *password = cJSON_GetObjectItem(user, "password")->valuestring;

    if (strlen(username) < 3 || strlen(username) > 32 || contains_space(username)) {
        error_handler(sock_fd, "Invalid username", 400);
        return -1;
    }

    if (strlen(name) < 3 || strlen(name) > 32) {
        error_handler(sock_fd, "Invalid name", 400);
        return -1;
    }

    if (strlen(password) < 3) {
        error_handler(sock_fd, "Invalid password", 400);
        return -1;
    }

    return 1;
}

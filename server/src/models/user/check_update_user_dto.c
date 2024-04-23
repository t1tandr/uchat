#include "server.h"

int check_update_user_dto(cJSON *user, int sock_fd) {
    if (user == NULL
        || (!cJSON_HasObjectItem(user, "username")
        && !cJSON_HasObjectItem(user, "name")
        && !cJSON_HasObjectItem(user, "avatar")
        && !cJSON_HasObjectItem(user, "bio")
        && !cJSON_HasObjectItem(user, "password"))) {
        error_handler(sock_fd, "Invalid json", 400);
        return -1;
    }

    if (cJSON_HasObjectItem(user, "username")) {
        char *username = cJSON_GetObjectItem(user, "username")->valuestring;

        if (strlen(username) < 3 || strlen(username) > 32 || contains_space(username)) {
            error_handler(sock_fd, "Invalid username", 400);
            return -1;
        }
    }

    if (cJSON_HasObjectItem(user, "name")) {
        char *name = cJSON_GetObjectItem(user, "name")->valuestring;

        if (strlen(name) < 3 || strlen(name) > 32) {
            error_handler(sock_fd, "Invalid name", 400);
            return -1;
        }
    }

    return 1;
}

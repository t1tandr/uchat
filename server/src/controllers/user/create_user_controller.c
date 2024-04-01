#include "server.h"

void create_user_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (data == NULL
        || !cJSON_HasObjectItem(data, "username")
        || !cJSON_HasObjectItem(data, "name")
        || !cJSON_HasObjectItem(data, "password")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    cJSON *user = create_user_service(data, db, sock_fd);

    if (!user) return;

    cJSON_DeleteItemFromObject(user, "password");

    send_response(sock_fd, user, 201);
}


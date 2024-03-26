#include "server.h"

void get_users_controller(sqlite3 *db, int sock_fd) {
    cJSON *users = get_users_service(db, sock_fd);

    if (users == NULL) return;

    send_response(sock_fd, users, 200);
}

void get_user_controller(int user_id, sqlite3 *db, int sock_fd) {
    cJSON *user = get_user_by_id_service(user_id, db, sock_fd);

    if (user == NULL) return;

    cJSON_DeleteItemFromObject(user, "password");

    send_response(sock_fd, user, 200);
}

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

void update_user_controller(int user_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (data == NULL
        || (!cJSON_HasObjectItem(data, "username")
        && !cJSON_HasObjectItem(data, "name")
        && !cJSON_HasObjectItem(data, "bio"))) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    cJSON *user = update_user_by_id_service(user_id, data, db, sock_fd);

    if (!user) return;

    cJSON_DeleteItemFromObject(user, "password");

    send_response(sock_fd, user, 201);
}

void delete_user_controller(int user_id, sqlite3 *db, int sock_fd) {
    cJSON *user = delete_user_by_id_service(user_id, db, sock_fd);

    if (user == NULL) return;

    cJSON_DeleteItemFromObject(user, "password");

    send_response(sock_fd, user, 200);
}

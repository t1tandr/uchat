#include "server.h"

void get_users_controller(sqlite3 *db, int sock_fd) {
    cJSON *users = get_users_service(db, sock_fd);

    if (users == NULL) return;

    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", 200);

    cJSON_AddItemToObject(res, "data", users);

    char *res_str = cJSON_Print(res);
    send(sock_fd, res_str, strlen(res_str), 0);

    cJSON_Delete(res);
    cJSON_free(res_str);
}

void get_user_controller(int user_id, sqlite3 *db, int sock_fd) {
    cJSON *user = get_user_by_id_service(user_id, db, sock_fd);

    if (user == NULL) return;

    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", 200);

    cJSON_DeleteItemFromObject(user, "password");
    cJSON_AddItemToObject(res, "data", user);

    char *res_str = cJSON_Print(res);
    send(sock_fd, res_str, strlen(res_str), 0);

    cJSON_Delete(res);
    cJSON_free(res_str);
}

void create_user_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (data == NULL
        || !cJSON_HasObjectItem(data, "username")
        || !cJSON_HasObjectItem(data, "password")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    char *username = cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring;
    char *password = cJSON_GetObjectItemCaseSensitive(data, "password")->valuestring; // IMPLEMENT PASSWORD HASHING

    cJSON *user = create_user_service(username, password, db, sock_fd);

    if (!user) return;

    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", 201);
    
    cJSON_DeleteItemFromObject(user, "password");
    cJSON_AddItemToObject(res, "data", user);

    char *res_str = cJSON_Print(res);
    send(sock_fd, res_str, strlen(res_str), 0);

    cJSON_Delete(res);
    cJSON_free(res_str);
}


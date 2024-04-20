#include "server.h"

void login_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (data == NULL
        || !cJSON_HasObjectItem(data, "username")
        || !cJSON_HasObjectItem(data, "password")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    cJSON *session_data = login_service(data, db, sock_fd);

    if (!session_data) return;

    cJSON_DeleteItemFromObject(session_data, "password");

    send_response(sock_fd, session_data, 200);
}


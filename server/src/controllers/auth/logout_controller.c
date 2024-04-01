#include "server.h"

void logout_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (data == NULL
        || !cJSON_HasObjectItem(data, "session_id")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    if (logout_service(data, db, sock_fd) == -1) return;

    send_response(sock_fd, NULL, 200);
}


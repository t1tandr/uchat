#include "server.h"

void logout_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    if (logout_service(headers, db, sock_fd) == -1) return;

    send_response(sock_fd, NULL, 200);
}


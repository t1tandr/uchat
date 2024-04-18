#include "server.h"

void delete_user_controller(int user_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    cJSON *user = delete_user_by_id_service(user_id, headers, db, sock_fd);

    if (user == NULL) return;

    cJSON_DeleteItemFromObject(user, "password");

    send_response(sock_fd, user, 200);
}


#include "server.h"

void get_user_controller(int user_id, sqlite3 *db, int sock_fd) {
    cJSON *user = get_user_by_id_service(user_id, db, sock_fd);

    if (user == NULL) return;

    cJSON_DeleteItemFromObject(user, "password");

    send_response(sock_fd, user, 200);
}


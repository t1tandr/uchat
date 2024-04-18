#include "server.h"

void create_user_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");
    if (check_user_dto(data, sock_fd) == -1) return;

    cJSON *user = create_user_service(data, db, sock_fd);
    if (user == NULL) return;

    cJSON_DeleteItemFromObject(user, "password");

    send_response(sock_fd, user, 201);
}


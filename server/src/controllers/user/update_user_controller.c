#include "server.h"

void update_user_controller(int user_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");
    
    if (check_update_user_dto(data, sock_fd) == -1) return;

    cJSON *user = update_user_by_id_service(user_id, data, headers, db, sock_fd);

    if (!user) return;

    cJSON_DeleteItemFromObject(user, "password");

    send_response(sock_fd, user, 201);
}


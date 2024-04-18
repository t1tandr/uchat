#include "server.h"

void update_chat_member_controller(int chat_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (check_update_chat_member_dto(data, sock_fd) == -1) return;

    cJSON *chat_member = update_chat_member_service(chat_id, data, headers, db, sock_fd);

    if (!chat_member) return;

    send_response(sock_fd, chat_member, 201);
}


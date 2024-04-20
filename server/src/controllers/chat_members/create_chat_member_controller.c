#include "server.h"

void create_chat_member_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (check_chat_member_dto(data, sock_fd) == -1) return;

    cJSON *chat_member = create_chat_member_service(data, headers, db, sock_fd);

    if (!chat_member) return;

    send_response(sock_fd, chat_member, 201);
}


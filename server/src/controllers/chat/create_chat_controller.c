#include "server.h"

void create_chat_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (check_chat_dto(data, sock_fd) == -1) return;

    cJSON *chat = create_chat_service(data, headers, db, sock_fd);

    if (!chat) return;

    send_response(sock_fd, chat, 201);
}


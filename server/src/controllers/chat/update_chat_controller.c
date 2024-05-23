#include "server.h"

void update_chat_controller(int chat_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (check_update_chat_dto(data, sock_fd) == -1) return;

    cJSON *chat = update_chat_by_id_service(chat_id, data, headers, db, sock_fd);

    if (!chat) return;

    send_response(sock_fd, chat, 201); // FIX: send all
}


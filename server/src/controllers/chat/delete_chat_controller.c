#include "server.h"

void delete_chat_controller(int chat_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    cJSON *chat = delete_chat_by_id_service(chat_id, headers, db, sock_fd);

    if (chat == NULL) return;

    send_response(sock_fd, chat, 200);
}


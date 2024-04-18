#include "server.h"

void get_chat_members_controller(int chat_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);

    if (!chat_members) return;

    send_response(sock_fd, chat_members, 200);
}


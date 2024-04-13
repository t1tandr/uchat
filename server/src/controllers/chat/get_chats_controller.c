#include "server.h"

void get_chats_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    if (headers == NULL
        || !cJSON_HasObjectItem(headers, "Authorization")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    cJSON *chats = get_chats_service(headers, db, sock_fd);

    if (!chats) return;

    send_response(sock_fd, chats, 201);
}


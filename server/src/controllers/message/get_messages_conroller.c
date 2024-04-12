#include "server.h"

void get_messages_controller(int chat_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    if (headers == NULL
        || !cJSON_HasObjectItem(headers, "Authorization")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    cJSON *messages = get_messages_service(chat_id, headers, db, sock_fd);

    if (!messages) return;

    send_response(sock_fd, messages, 200);
}


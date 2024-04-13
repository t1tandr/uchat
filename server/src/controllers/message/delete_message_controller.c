#include "server.h"

void delete_message_controller(int message_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    if (headers == NULL
        || !cJSON_HasObjectItem(headers, "Authorization")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    cJSON *message = delete_message_by_id_service(message_id, headers, db, sock_fd);

    if (!message) return;

    send_response(sock_fd, message, 201);
}


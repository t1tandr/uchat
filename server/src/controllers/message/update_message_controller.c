#include "server.h"

void update_message_controller(int message_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (data == NULL
        || headers == NULL
        || !cJSON_HasObjectItem(headers, "Authorization")
        || !cJSON_HasObjectItem(data, "text")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    cJSON *message = update_message_service(message_id, data, headers, db, sock_fd);

    if (!message) return;

    send_response(sock_fd, message, 201);
}


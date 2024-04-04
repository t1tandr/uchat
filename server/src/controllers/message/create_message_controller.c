#include "server.h"

void create_message_controller(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (data == NULL
        || headers == NULL
        || !cJSON_HasObjectItem(headers, "Authorization")
        || (!cJSON_HasObjectItem(data, "chat_id")
        && !cJSON_HasObjectItem(data, "text"))) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    cJSON *message = create_message_service(data, headers, db, sock_fd);

    if (!message) return;

    send_response_message_all(sock_fd, db, message, 201);
}


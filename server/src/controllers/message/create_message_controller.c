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

    cJSON *chat_members = get_chat_members_service(
        cJSON_GetObjectItem(message, "chat_id")->valueint,
        headers,
        db,
        sock_fd
    );
    
    if (!chat_members) return;

    GHashTable *user_ids = chat_members_to_user_ids_set(chat_members);

    cJSON_Delete(chat_members);

    send_response_users_by_id(user_ids, message, 201);
    // send_response_message_all(headers, message, 201, sock_fd, db);
}


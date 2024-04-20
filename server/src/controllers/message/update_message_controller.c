#include "server.h"

void update_message_controller(int message_id, cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");
    cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

    if (check_update_message_dto(data, sock_fd) == -1) return;

    cJSON *message = update_message_service(message_id, data, headers, db, sock_fd);

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
}


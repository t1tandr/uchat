#include "server.h"

cJSON *create_message_service(cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");

    int user_id = cJSON_GetObjectItemCaseSensitive(session, "user_id")->valueint;
    int chat_id = cJSON_GetObjectItemCaseSensitive(data, "chat_id")->valueint;
    char *type = cJSON_GetObjectItemCaseSensitive(data, "type")->valuestring;
    char *content = cJSON_GetObjectItemCaseSensitive(data, "content")->valuestring;

    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);

    if (!is_user_chat_member(user_id, chat_members)) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    if (strcmp(type, "photo") == 0) {
        content = create_image(content);
    }

    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "INSERT INTO messages (user_id, chat_id, type, content)"
        "VALUES (%d, %d, %Q, %Q) RETURNING *;",
        user_id,
        chat_id,
        type,
        content
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *message = stmt_to_message_json(stmt);
    cJSON *user = get_user_by_id_service(user_id, db, sock_fd);
    char *username = cJSON_GetObjectItem(user, "username")->valuestring;

    cJSON_AddStringToObject(message, "username", mx_strdup(username));
    
    if (strcmp(type, "photo") == 0) {
        char *image_id = cJSON_GetObjectItem(message, "content")->valuestring;
        long size;
        unsigned char *image = get_image(image_id, &size);

        char *base64 = g_base64_encode(image, size);

        cJSON_ReplaceItemInObject(message, "content", cJSON_CreateString(base64));
    }

    cJSON_Delete(user);
    cJSON_Delete(chat_members);
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return message;
}


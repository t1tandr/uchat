#include "server.h"

cJSON *delete_message_by_id_service(int message_id, cJSON *headers, sqlite3 *db, int sock_fd) {
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");

    int user_id = cJSON_GetObjectItem(session, "user_id")->valueint;

    cJSON *message = get_message_by_id_service(message_id, headers, db, sock_fd);
    if (message == NULL) {
        return NULL;
    }

    if (user_id != cJSON_GetObjectItem(message, "user_id")->valueint) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    int chat_id = cJSON_GetObjectItem(message, "chat_id")->valueint;
    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);
    if (chat_members == NULL) {
        return NULL;
    }

    if (!is_user_chat_member(user_id, chat_members)) {
        error_handler(sock_fd, "User is not in chat", 400);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "DELETE FROM messages WHERE id=%d RETURNING *;",
        message_id
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        sqlite3_free(sql);
        return NULL;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    char *type = cJSON_GetObjectItem(message, "type")->valuestring;
    if (strcmp(type, "photo") == 0) {
        char *photo_id = cJSON_GetObjectItem(message, "content")->valuestring;
        delete_image(photo_id);
    }

    cJSON *deleted_message = stmt_to_message_json(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return deleted_message;
}


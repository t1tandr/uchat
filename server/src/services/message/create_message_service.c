#include "server.h"

cJSON *create_message_service(cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
    char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;
    cJSON *session = get_session(session_id, db);

    if (session == NULL) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    int user_id = cJSON_GetObjectItemCaseSensitive(session, "user_id")->valueint;
    int chat_id = cJSON_GetObjectItemCaseSensitive(data, "chat_id")->valueint;
    char *text = cJSON_GetObjectItemCaseSensitive(data, "text")->valuestring;

    cJSON *chat_members = get_chat_members_service(chat_id, db, sock_fd);

    if (!is_user_chat_member(user_id, chat_members)) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "INSERT INTO messages (user_id, chat_id, text)"
        "VALUES (%d, %d, %Q) RETURNING *;",
        user_id,
        chat_id,
        text
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
    
    cJSON_Delete(chat_members);
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return message;
}


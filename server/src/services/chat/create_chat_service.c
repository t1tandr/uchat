#include "server.h"

cJSON *create_chat_service(cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
    char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;
    cJSON *session = get_session(session_id, db);

    if (session == NULL) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char *sql;

    // int user_id = cJSON_GetObjectItemCaseSensitive(session, "user_id")->valueint;
    char *name = cJSON_GetObjectItemCaseSensitive(data, "name")->valuestring;

    sql = sqlite3_mprintf(
        "INSERT INTO chats (name)"
        "VALUES (%Q) RETURNING *;",
        name
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    // add user to chat_members

    cJSON *chat = stmt_to_chat_json(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return chat;
}


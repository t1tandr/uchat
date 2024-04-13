#include "server.h"

cJSON *get_messages_service(int chat_id, cJSON *headers, sqlite3 *db, int sock_fd) {
    char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;
    cJSON *session = get_session(session_id, db);

    if (session == NULL) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    int user_id = cJSON_GetObjectItemCaseSensitive(session, "user_id")->valueint;

    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);

    if (!is_user_chat_member(user_id, chat_members)) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "SELECT * FROM messages WHERE chat_id=%d;",
        chat_id
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        sqlite3_free(sql);
        return NULL;
    }

    cJSON *messages = cJSON_CreateArray();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON *message = stmt_to_message_json(stmt);

        cJSON_AddItemToArray(messages, message);
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return messages;
}


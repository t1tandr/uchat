#include "server.h"

cJSON *get_chats_service(cJSON *headers, sqlite3 *db, int sock_fd) {
    char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;
    cJSON *session = get_session(session_id, db);

    if (session == NULL) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char *sql;

    int user_id = cJSON_GetObjectItemCaseSensitive(session, "user_id")->valueint;

    sql = sqlite3_mprintf(
        "SELECT * FROM chats "
        "WHERE id IN (SELECT chat_id FROM chat_members WHERE user_id = %d);",
        user_id
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    cJSON *chats = cJSON_CreateArray();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON *chat = stmt_to_chat_json(stmt);
        cJSON_AddItemToArray(chats, chat);
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return chats;
}


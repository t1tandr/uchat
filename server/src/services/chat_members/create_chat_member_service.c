#include "server.h"

cJSON *create_chat_member_service(cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");

    int user_id = cJSON_GetObjectItem(session, "user_id")->valueint;
    int chat_id = cJSON_GetObjectItem(data, "chat_id")->valueint;
    int add_user_id = cJSON_GetObjectItem(data, "user_id")->valueint;

    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);

    if (!is_user_chat_member(user_id, chat_members) && !(cJSON_GetArraySize(chat_members) == 0 && user_id == add_user_id)) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    if (is_user_chat_member(add_user_id, chat_members)) {
        error_handler(sock_fd, "User is already in chat", 400);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "INSERT INTO chat_members (chat_id, user_id) VALUES (%d, %d) RETURNING *;",
        chat_id,
        add_user_id
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

    cJSON *chat_member = stmt_to_chat_member_json(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return chat_member;
}


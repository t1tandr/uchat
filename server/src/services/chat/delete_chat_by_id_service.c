#include "server.h"

cJSON *delete_chat_by_id_service(int chat_id, cJSON *headers, sqlite3 *db, int sock_fd) {
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");

    int user_id = cJSON_GetObjectItemCaseSensitive(session, "user_id")->valueint;

    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);

    if (!is_user_chat_member(user_id, chat_members)) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "DELETE FROM chats WHERE id = %d RETURNING *;",
        chat_id
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

    cJSON *chat = stmt_to_chat_json(stmt);
    
    if (cJSON_HasObjectItem(chat, "img")) {
        char *old_photo_id = cJSON_GetObjectItem(chat, "img")->valuestring;
        delete_image(old_photo_id);
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return chat;
}


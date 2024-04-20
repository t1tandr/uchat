#include "server.h"

cJSON *delete_user_by_id_service(int user_id, cJSON *headers, sqlite3 *db, int sock_fd) {
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");

    if (cJSON_GetObjectItem(session, "user_id")->valueint != user_id) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "DELETE FROM users WHERE id = %d RETURNING *;",
        user_id
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

    cJSON *user = stmt_to_user_json(stmt);

    if (cJSON_HasObjectItem(user, "avatar")) {
        char *old_photo_id = cJSON_GetObjectItem(user, "avatar")->valuestring;
        delete_image(old_photo_id);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return user;
}


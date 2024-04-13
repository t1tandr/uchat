#include "server.h"

cJSON *update_message_service(int message_id, cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
    char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;
    cJSON *session = get_session(session_id, db);

    if (session == NULL) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char sql[1024];

    int user_id = cJSON_GetObjectItem(session, "user_id")->valueint;
    char *update_text = cJSON_GetObjectItem(data, "text")->valuestring;

    cJSON *message = get_message_by_id_service(message_id, headers, db, sock_fd);
    if (message == NULL) {
        return NULL;
    }

    if (user_id != cJSON_GetObjectItem(message, "user_id")->valueint) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    strcpy(sql, "UPDATE messages SET ");

    if (cJSON_HasObjectItem(data, "text")) {
        strcat(sql, "text = ?, ");
    }

    strcat(sql, "updated_at = CURRENT_TIMESTAMP ");

    strcat(sql, "WHERE id = ? RETURNING *;");

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    int param_index = 1;
    if (cJSON_HasObjectItem(data, "text")) {
        sqlite3_bind_text(stmt, param_index++, update_text, -1, SQLITE_TRANSIENT);
    }

    sqlite3_bind_int(stmt, param_index, message_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *new_message = stmt_to_message_json(stmt);
    
    sqlite3_finalize(stmt);

    return new_message;
}


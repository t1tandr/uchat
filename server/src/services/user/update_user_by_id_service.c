#include "server.h"

cJSON *update_user_by_id_service(int user_id, cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
    char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;
    if (!session_exists(session_id, user_id, db)) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char sql[1024];

    strcpy(sql, "UPDATE users SET ");

    if (cJSON_HasObjectItem(data, "username")) {
        strcat(sql, "username = ?, ");
    }

    if (cJSON_HasObjectItem(data, "name")) {
        strcat(sql, "name = ?, ");
    }

    if (cJSON_HasObjectItem(data, "bio")) {
        strcat(sql, "bio = ?, ");
    }

    sql[strlen(sql) - 2] = '\0';
    strcat(sql, "WHERE id = ? RETURNING *;");
 
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    int param_index = 1;
    if (cJSON_HasObjectItem(data, "username")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "username")->valuestring, -1, SQLITE_TRANSIENT);
    }

    if (cJSON_HasObjectItem(data, "name")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "name")->valuestring, -1, SQLITE_TRANSIENT);
    }

    if (cJSON_HasObjectItem(data, "bio")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "bio")->valuestring, -1, SQLITE_TRANSIENT);
    }

    sqlite3_bind_int(stmt, param_index, user_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *user = stmt_to_user_json(stmt);
    
    sqlite3_finalize(stmt);

    return user;
}


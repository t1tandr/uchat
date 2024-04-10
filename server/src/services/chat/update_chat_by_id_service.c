#include "server.h"

cJSON *update_chat_by_id_service(int chat_id, cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
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
    char sql[1024];

    strcpy(sql, "UPDATE chats SET ");

    if (cJSON_HasObjectItem(data, "name")) {
        strcat(sql, "name = ?, ");
    }

    if (cJSON_HasObjectItem(data, "img")) {
        strcat(sql, "img = ?, ");
    }

    if (cJSON_HasObjectItem(data, "link")) {
        strcat(sql, "link = ?, ");
    }

    sql[strlen(sql) - 2] = '\0';
    strcat(sql, "WHERE id = ? RETURNING *;");
 
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    int param_index = 1;
    if (cJSON_HasObjectItem(data, "name")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "name")->valuestring, -1, SQLITE_TRANSIENT);
    }

    if (cJSON_HasObjectItem(data, "img")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "img")->valuestring, -1, SQLITE_TRANSIENT);
    }

    if (cJSON_HasObjectItem(data, "link")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "link")->valuestring, -1, SQLITE_TRANSIENT);
    }

    sqlite3_bind_int(stmt, param_index, chat_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *chat = stmt_to_chat_json(stmt);
    
    sqlite3_finalize(stmt);

    return chat;
}


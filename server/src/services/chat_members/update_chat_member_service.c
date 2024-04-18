#include "server.h"

cJSON *update_chat_member_service(int chat_id, cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");

    sqlite3_stmt *stmt;
    char sql[1024];

    int user_id = cJSON_GetObjectItem(session, "user_id")->valueint;
    int update_user_id = cJSON_GetObjectItem(data, "user_id")->valueint;
    char *update_role = cJSON_GetObjectItem(data, "role")->valuestring;

    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);

    if (!is_user_chat_member(user_id, chat_members) || !is_user_chat_member(update_user_id, chat_members)) {
        error_handler(sock_fd, "Invalid data", 400);
        return NULL;
    }

    for (int i = 0; i < cJSON_GetArraySize(chat_members); i++) {
        cJSON *chat_member = cJSON_GetArrayItem(chat_members, i);
        int chat_member_id = cJSON_GetObjectItem(chat_member, "user_id")->valueint;
        char *chat_member_role = cJSON_GetObjectItem(chat_member, "role")->valuestring;

        if (chat_member_id == user_id && strcmp(chat_member_role, "ADMIN") != 0 && cJSON_GetArraySize(chat_members) > 1) {
            error_handler(sock_fd, "Invalid permissions", 400);
            return NULL;
        }
    }

    strcpy(sql, "UPDATE chat_members SET ");

    if (cJSON_HasObjectItem(data, "role")) {
        strcat(sql, "role = ?, ");
    }

    sql[strlen(sql) - 2] = '\0';
    strcat(sql, "WHERE chat_id = ? AND user_id = ? RETURNING *;");
 
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    int param_index = 1;
    if (cJSON_HasObjectItem(data, "role")) {
        sqlite3_bind_text(stmt, param_index++, update_role, -1, SQLITE_TRANSIENT);
    }

    sqlite3_bind_int(stmt, param_index++, chat_id);
    sqlite3_bind_int(stmt, param_index, update_user_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *chat_member = stmt_to_chat_member_json(stmt);
    
    sqlite3_finalize(stmt);

    return chat_member;
}


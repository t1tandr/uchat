#include "server.h"

cJSON *get_messages_service(int chat_id, cJSON *headers, sqlite3 *db, int sock_fd) {
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");
    cJSON *query_params = cJSON_GetObjectItem(headers, "query_params");

    int user_id = cJSON_GetObjectItemCaseSensitive(session, "user_id")->valueint;

    cJSON *chat_members = get_chat_members_service(chat_id, headers, db, sock_fd);

    if (!is_user_chat_member(user_id, chat_members)) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    sqlite3_stmt *stmt;
    char sql[1024];

    strcpy(sql, "SELECT * FROM (SELECT * FROM messages JOIN users ON messages.user_id = users.id WHERE chat_id=? ORDER BY id DESC");

    if (query_params != NULL && cJSON_HasObjectItem(query_params, "limit")) {
        strcat(sql, " LIMIT ? ");

        if (query_params != NULL && cJSON_HasObjectItem(query_params, "offset")) {
            strcat(sql, " OFFSET ? ");
        }
    }
    strcat(sql, ") ORDER BY id ASC;");

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    int param_index = 1;
    sqlite3_bind_int(stmt, param_index++, chat_id);

    if (query_params != NULL && cJSON_HasObjectItem(query_params, "limit")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(query_params, "limit")->valuestring, -1, SQLITE_TRANSIENT);
    }

    if (query_params != NULL && cJSON_HasObjectItem(query_params, "offset")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(query_params, "offset")->valuestring, -1, SQLITE_TRANSIENT);
    }

    cJSON *messages = cJSON_CreateArray();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON *message = stmt_to_message_json(stmt);

        cJSON_AddStringToObject(message, "username", (const char *) sqlite3_column_text(stmt, 8));

        char *type = cJSON_GetObjectItem(message, "type")->valuestring;

        if (strcmp(type, "photo") == 0) {
            char *image_id = cJSON_GetObjectItem(message, "content")->valuestring;
            long size;
            unsigned char *image = get_image(image_id, &size);

            char *base64 = g_base64_encode(image, size);

            cJSON_ReplaceItemInObject(message, "content", cJSON_CreateString(base64));
        }

        cJSON_AddItemToArray(messages, message);
    }

    sqlite3_finalize(stmt);

    return messages;
}


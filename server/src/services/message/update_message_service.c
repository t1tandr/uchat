#include "server.h"

cJSON *update_message_service(int message_id, cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) {
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");

    sqlite3_stmt *stmt;
    char sql[1024];

    int user_id = cJSON_GetObjectItem(session, "user_id")->valueint;
    char *update_content = cJSON_GetObjectItem(data, "content")->valuestring;

    cJSON *message = get_message_by_id_service(message_id, headers, db, sock_fd);
    if (message == NULL) {
        return NULL;
    }

    if (user_id != cJSON_GetObjectItem(message, "user_id")->valueint) {
        error_handler(sock_fd, "Unauthorized", 401);
        return NULL;
    }

    char *type = cJSON_GetObjectItem(message, "type")->valuestring;
    if (strcmp(type, "photo") == 0) {
        char *old_photo_id = cJSON_GetObjectItem(message, "content")->valuestring;
        delete_image(old_photo_id);

        update_content = create_image(update_content);
    }

    strcpy(sql, "UPDATE messages SET ");

    if (cJSON_HasObjectItem(data, "content")) {
        strcat(sql, "content = ?, ");
    }

    strcat(sql, "updated_at = CURRENT_TIMESTAMP ");

    strcat(sql, "WHERE id = ? RETURNING *;");

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    int param_index = 1;
    if (cJSON_HasObjectItem(data, "content")) {
        sqlite3_bind_text(stmt, param_index++, update_content, -1, SQLITE_TRANSIENT);
    }

    sqlite3_bind_int(stmt, param_index, message_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *new_message = stmt_to_message_json(stmt);
    cJSON *user = get_user_by_id_service(user_id, db, sock_fd);
    char *username = cJSON_GetObjectItem(user, "username")->valuestring;

    cJSON_AddStringToObject(new_message, "username", mx_strdup(username));

    cJSON_Delete(user);

    sqlite3_finalize(stmt);

    if (strcmp(type, "photo") == 0) {
        char *image_id = cJSON_GetObjectItem(new_message, "content")->valuestring;
        long size;
        unsigned char *image = get_image(image_id, &size);

        char *base64 = g_base64_encode(image, size);

        cJSON_ReplaceItemInObject(new_message, "content", cJSON_CreateString(base64));
    }

    return new_message;
}


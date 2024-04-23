#include "server.h"

cJSON *create_chat_service(cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd) { // TODO: do transaction
    cJSON *session = cJSON_GetObjectItem(headers, "session_data");

    sqlite3_stmt *stmt;
    char *sql;

    int user_id = cJSON_GetObjectItem(session, "user_id")->valueint;
    char *name = cJSON_GetObjectItem(data, "name")->valuestring;
    char *img = NULL;

    if (cJSON_HasObjectItem(data, "img")) {
        char *new_img_base64 = cJSON_GetObjectItem(data, "img")->valuestring;
        img = create_image(new_img_base64);
    }

    sql = sqlite3_mprintf(
        "INSERT INTO chats (name, img) VALUES (%Q, %Q) RETURNING *;",
        name,
        img
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *chat = stmt_to_chat_json(stmt);

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    int chat_id = cJSON_GetObjectItem(chat, "id")->valueint;
    cJSON *chat_member_data = cJSON_CreateObject();
    cJSON_AddNumberToObject(chat_member_data, "chat_id", chat_id);
    cJSON_AddNumberToObject(chat_member_data, "user_id", user_id);

    cJSON *chat_member = create_chat_member_service(chat_member_data, headers, db, sock_fd);
    if (chat_member == NULL) {
        return NULL;
    }

    cJSON_AddStringToObject(chat_member_data, "role", "ADMIN");
    chat_member = update_chat_member_service(chat_id, chat_member_data, headers, db, sock_fd);
    if (chat_member == NULL) {
        return NULL;
    }

    cJSON_Delete(chat_member);
    cJSON_Delete(chat_member_data);

    return chat;
}


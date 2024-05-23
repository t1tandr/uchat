#include "server.h"

cJSON *get_chat_by_id_service(int chat_id, sqlite3 *db, int sock_fd) {
    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "SELECT * FROM chats WHERE id=%d;",
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
        char *image_id = cJSON_GetObjectItem(chat, "img")->valuestring;
        long size;
        unsigned char *image = get_image(image_id, &size);

        char *base64 = g_base64_encode(image, size);

        cJSON_ReplaceItemInObject(chat, "img", cJSON_CreateString(base64));
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return chat;
}


#include "server.h"

cJSON *get_chats_service(cJSON *headers, sqlite3 *db, int sock_fd) {
    sqlite3_stmt *stmt;
    char *sql;

    cJSON *session = cJSON_GetObjectItem(headers, "session_data");
    int user_id = cJSON_GetObjectItemCaseSensitive(session, "user_id")->valueint;

    sql = sqlite3_mprintf(
        "SELECT * FROM chats "
        "WHERE id IN (SELECT chat_id FROM chat_members WHERE user_id = %d);",
        user_id
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    cJSON *chats = cJSON_CreateArray();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON *chat = stmt_to_chat_json(stmt);

        if (cJSON_HasObjectItem(chat, "img")) {
            char *image_id = cJSON_GetObjectItem(chat, "img")->valuestring;
            long size;
            unsigned char *image = get_image(image_id, &size);

            char *base64 = g_base64_encode(image, size);

            cJSON_ReplaceItemInObject(chat, "img", cJSON_CreateString(base64));
        }

        cJSON_AddItemToArray(chats, chat);
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return chats;
}


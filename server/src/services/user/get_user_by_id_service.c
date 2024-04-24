#include "server.h"

cJSON *get_user_by_id_service(int user_id, sqlite3 *db, int sock_fd) {
    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "SELECT * FROM users WHERE id=%d;",
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
        char *image_id = cJSON_GetObjectItem(user, "avatar")->valuestring;
        long size;
        unsigned char *image = get_image(image_id, &size);

        char *base64 = g_base64_encode(image, size);

        cJSON_ReplaceItemInObject(user, "avatar", cJSON_CreateString(base64));
    }

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return user;
}


#include "server.h"

cJSON *get_users_service(sqlite3 *db, int sock_fd) {
    sqlite3_stmt *stmt;
    char *sql;

    sql = "SELECT * FROM users;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        sqlite3_free(sql);
        return NULL;
    }
    
    cJSON *users = cJSON_CreateArray();

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        cJSON *user = stmt_to_user_json(stmt);

        if (cJSON_HasObjectItem(user, "avatar")) {
            char *image_id = cJSON_GetObjectItem(user, "avatar")->valuestring;
            long size;
            unsigned char *image = get_image(image_id, &size);

            char *base64 = g_base64_encode(image, size);

            cJSON_ReplaceItemInObject(user, "avatar", cJSON_CreateString(base64));
        }
        
        cJSON_AddItemToArray(users, user);
    }

    sqlite3_finalize(stmt);

    return users;
}


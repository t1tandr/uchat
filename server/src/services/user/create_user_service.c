#include "server.h"

cJSON *create_user_service(cJSON *data, sqlite3 *db, int sock_fd) {
    char salt[BCRYPT_HASHSIZE];
    char hash[BCRYPT_HASHSIZE];

    sqlite3_stmt *stmt;
    char *sql;

    char *username = cJSON_GetObjectItem(data, "username")->valuestring;
    char *name = cJSON_GetObjectItem(data, "name")->valuestring;
    char *password = cJSON_GetObjectItem(data, "password")->valuestring;

    cJSON *user = get_user_by_username_service(username, db, sock_fd);

    if (user != NULL) {
        error_handler(sock_fd, "User already exists", 400);
        return NULL;
    }

    if (bcrypt_gensalt(12, salt) != 0) {
        error_handler(sock_fd, "Internal error", 500);
        return NULL;
    }

    if (bcrypt_hashpw(password, salt, hash) != 0) {
        error_handler(sock_fd, "Bad data", 422);
        return NULL;
    }

    sql = sqlite3_mprintf(
        "INSERT INTO users (username, name, password)"
        "VALUES(%Q, %Q, %Q) RETURNING *;",
        username,
        name,
        hash
    );
   
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        sqlite3_finalize(stmt);
        sqlite3_free(sql);
        return NULL;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        sqlite3_finalize(stmt);
        sqlite3_free(sql);
        return NULL;
    }

    user = stmt_to_user_json(stmt);

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return user;
}


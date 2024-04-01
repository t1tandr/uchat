#include "server.h"

cJSON *create_user_service(cJSON *data, sqlite3 *db, int sock_fd) {
    char salt[BCRYPT_HASHSIZE];
    char hash[BCRYPT_HASHSIZE];

    int rc;
    char *sql, *error_message;

    char *username = cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring;
    char *name = cJSON_GetObjectItemCaseSensitive(data, "name")->valuestring;
    char *password = cJSON_GetObjectItemCaseSensitive(data, "password")->valuestring;

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
        "VALUES(%Q, %Q, %Q);",
        username,
        name,
        hash
    );
   
    rc = sqlite3_exec(db, sql, NULL, 0, &error_message);
    sqlite3_free(sql);

    if (rc != SQLITE_OK){
        error_handler(sock_fd, error_message, 422);
        sqlite3_free(error_message);
        return NULL;
    }

    cJSON *user = get_user_by_username_service(username, db, sock_fd);

    if (!user) return NULL;

    return user;
}


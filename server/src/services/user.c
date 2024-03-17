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
        cJSON *user = cJSON_CreateObject();
        cJSON_AddNumberToObject(user, "id", sqlite3_column_int(stmt, 0));
        cJSON_AddStringToObject(user, "username", (const char *) sqlite3_column_text(stmt, 1));
        cJSON_AddItemToArray(users, user);
    }

    sqlite3_finalize(stmt);

    return users;
}

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

    cJSON *user = cJSON_CreateObject();

    cJSON_AddNumberToObject(user, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(user, "username", (const char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 2));

    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return user;
}

cJSON *get_user_by_username_service(char* username, sqlite3 *db, int sock_fd) {
    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "SELECT * FROM users WHERE username=%Q",
        username
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

    cJSON *user = cJSON_CreateObject();

    cJSON_AddNumberToObject(user, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(user, "username", (const char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 2));
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return user;
}

cJSON *create_user_service(char *username, char* password, sqlite3 *db, int sock_fd) {
    int rc;
    char *sql, *error_message;

    sql = sqlite3_mprintf(
        "INSERT INTO users (username, password)"
        "VALUES(%Q, %Q);",
        username,
        password
    );
   
    rc = sqlite3_exec(db, sql, NULL, 0, &error_message);
    sqlite3_free(sql);

    if (rc != SQLITE_OK){
        error_handler(sock_fd, error_message, 422);
        sqlite3_free(error_message);
        return NULL;
    }

    sqlite3_free(sql);

    cJSON *user = get_user_by_username_service(username, db, sock_fd);

    if (!user) return NULL;

    return user;
}

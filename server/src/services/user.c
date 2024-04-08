#include "server.h"
#include <bcrypt.h>

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
        cJSON_AddStringToObject(user, "name", (const char *) sqlite3_column_text(stmt, 2));
        cJSON_AddStringToObject(user, "bio", (const char *) sqlite3_column_text(stmt, 3));
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
    cJSON_AddStringToObject(user, "name", (const char *) sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(user, "bio", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 4));

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
    cJSON_AddStringToObject(user, "name", (const char *) sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(user, "bio", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 4));
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return user;
}

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

cJSON *update_user_by_id_service(int user_id, cJSON *data, sqlite3 *db, int sock_fd) {
    sqlite3_stmt *stmt;
    char sql[1024];

    strcpy(sql, "UPDATE users SET ");

    if (cJSON_HasObjectItem(data, "username")) {
        strcat(sql, "username = ?, ");
    }

    if (cJSON_HasObjectItem(data, "name")) {
        strcat(sql, "name = ?, ");
    }

    if (cJSON_HasObjectItem(data, "bio")) {
        strcat(sql, "bio = ?, ");
    }

    sql[strlen(sql) - 2] = '\0';
    strcat(sql, "WHERE id = ? RETURNING *;");
 
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        return NULL;
    }

    int param_index = 1;
    if (cJSON_HasObjectItem(data, "username")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "username")->valuestring, -1, SQLITE_TRANSIENT);
    }

    if (cJSON_HasObjectItem(data, "name")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "name")->valuestring, -1, SQLITE_TRANSIENT);
    }

    if (cJSON_HasObjectItem(data, "bio")) {
        sqlite3_bind_text(stmt, param_index++, cJSON_GetObjectItem(data, "bio")->valuestring, -1, SQLITE_TRANSIENT);
    }

    sqlite3_bind_int(stmt, param_index, user_id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *user = cJSON_CreateObject();

    cJSON_AddNumberToObject(user, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(user, "username", (const char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(user, "name", (const char *) sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(user, "bio", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 4));
    
    sqlite3_finalize(stmt);

    return user;
}

cJSON *delete_user_by_id_service(int user_id, sqlite3 *db, int sock_fd) {
    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "DELETE FROM users WHERE id = %d RETURNING *;",
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
    cJSON_AddStringToObject(user, "name", (const char *) sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(user, "bio", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 4));
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return user;
}

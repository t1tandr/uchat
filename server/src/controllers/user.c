#include "server.h"

void create_user_controller(cJSON *data, sqlite3 *db, int sock_fd) {
    if (!cJSON_HasObjectItem(data, "username") || !cJSON_HasObjectItem(data, "password")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    sqlite3_stmt *stmt;
    int rc;
    char *sql, *error_message;

    char *username = cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring;
    char *password = cJSON_GetObjectItemCaseSensitive(data, "password")->valuestring; // IMPLEMENT PASSWORD HASHING
    
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
        return;
    }

    sql = sqlite3_mprintf(
        "SELECT * FROM users WHERE username=%Q",
        username
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422);
        sqlite3_free(sql);
        return;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        error_handler(sock_fd, "Invalid input data", 422);
        return;
    }

    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", 201);

    cJSON *res_data = cJSON_CreateObject();
    cJSON_AddNumberToObject(res_data, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(res_data, "username", (const char *) sqlite3_column_text(stmt, 1));
    
    cJSON_AddItemToObject(res, "data", res_data);

    char *res_str = cJSON_Print(res);
    send(sock_fd, res_str, strlen(res_str), 0);

    sqlite3_finalize(stmt);
    cJSON_Delete(res);
    cJSON_free(res_str);
    sqlite3_free(sql);
}


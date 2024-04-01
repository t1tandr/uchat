#include "server.h"

bool is_file_exists(char *filename) {
    return access(filename, F_OK) == 0;
}

cJSON *stmt_to_user_json(sqlite3_stmt *stmt) {
    cJSON *user = cJSON_CreateObject();

    cJSON_AddNumberToObject(user, "id", sqlite3_column_int(stmt, 0));
    cJSON_AddStringToObject(user, "username", (const char *) sqlite3_column_text(stmt, 1));
    cJSON_AddStringToObject(user, "name", (const char *) sqlite3_column_text(stmt, 2));
    cJSON_AddStringToObject(user, "bio", (const char *) sqlite3_column_text(stmt, 3));
    cJSON_AddStringToObject(user, "password", (const char *) sqlite3_column_text(stmt, 4));
    cJSON_AddStringToObject(user, "created_at", (const char *) sqlite3_column_text(stmt, 5));

    return user;
}


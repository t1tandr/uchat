#include "server.h"

void create_user_controller(cJSON *data, sqlite3 *db) {
    char *username = cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring;
    char *password = cJSON_GetObjectItemCaseSensitive(data, "password")->valuestring;
    
    char *sql = sqlite3_mprintf(
        "INSERT INTO users (username, password)"
        "VALUES(%Q, %Q);",
        username,
        password
    );
   
    char *error_message;
    int rc = sqlite3_exec(db, sql, NULL, 0, &error_message);

    if (rc != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}


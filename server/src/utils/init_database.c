#include "server.h"

void init_database() {
    if (is_file_exists("uchat.db")) return;

    sqlite3 *db;
    char *error_message;
    int rc;

    db = database_connect();

    char *sql =
        "CREATE TABLE users ("
        "id INTEGER PRIMARY KEY NOT NULL,"
        "username VARCHAR(30) NOT NULL UNIQUE,"
        "name VARCHAR(30) NOT NULL,"
        "bio VARCHAR(200),"
        "password TEXT NOT NULL);";

    rc = sqlite3_exec(db, sql, NULL, 0, &error_message);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
    } else {
        fprintf(stdout, "Database initiated successfully\n");
    }

    sqlite3_close(db);
}


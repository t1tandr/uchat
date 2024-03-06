#include "server.h"

void init_database() {
    if (is_file_exists("uchat.db")) return;

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    db = database_connect();

    char *sql =
        "CREATE TABLE users ("
        "id INTEGER PRIMARY KEY NOT NULL,"
        "username VARCHAR(30) NOT NULL UNIQUE,"
        "password TEXT NOT NULL);";

    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    sqlite3_close(db);
}


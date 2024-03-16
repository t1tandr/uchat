#include "server.h"

sqlite3 *database_connect() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("uchat.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    return db;
}

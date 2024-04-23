#include "server.h"

cJSON *get_user_by_username_service(char* username, sqlite3 *db, int sock_fd) {
    (void) sock_fd;
    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "SELECT * FROM users WHERE username=%Q",
        username
    );

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        // error_handler(sock_fd, (char *) sqlite3_errmsg(db), 422); // Bad decision to comment
        sqlite3_free(sql);
        return NULL;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        // error_handler(sock_fd, "Invalid input data", 422);
        return NULL;
    }

    cJSON *user = stmt_to_user_json(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    return user;
}


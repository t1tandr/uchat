#include "server.h"

bool session_exists(char *session_id, int user_id, sqlite3 *db) {
    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "SELECT * FROM sessions WHERE id = %Q;",
        session_id
    );

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);

    int sql_result = sqlite3_step(stmt);

    if (sql_result != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return false;
    }

    bool is_equal_id = sqlite3_column_int(stmt, 1) == user_id;
    sqlite3_finalize(stmt);

    return user_id == -1 ? true : is_equal_id;
}


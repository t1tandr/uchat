#include "server.h"

cJSON *get_session(char *session_id, sqlite3 *db) {
    sqlite3_stmt *stmt;
    char *sql;

    sql = sqlite3_mprintf(
        "SELECT * FROM sessions WHERE id = %Q;",
        session_id
    );

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_free(sql);


    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    cJSON *session_data = cJSON_CreateObject();
    cJSON_AddStringToObject(session_data, "session_id", (const char *) sqlite3_column_text(stmt, 0));
    cJSON_AddNumberToObject(session_data, "user_id", sqlite3_column_int(stmt, 1));

    sqlite3_finalize(stmt);

    return session_data; 
}


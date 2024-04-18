#include "server.h"

int logout_service(cJSON *headers, sqlite3 *db, int sock_fd) {
    int rc;
    char *sql, *error_message;

    char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;

    sql = sqlite3_mprintf(
        "DELETE FROM sessions WHERE id = %Q;",
        session_id
    );

    rc = sqlite3_exec(db, sql, NULL, 0, &error_message);
    sqlite3_free(sql);

    if (rc != SQLITE_OK){
        error_handler(sock_fd, error_message, 422);
        sqlite3_free(error_message);
        return -1;
    }

    remove_client_connection(session_id);

    return 1;
}


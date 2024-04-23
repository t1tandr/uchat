#include "server.h"

cJSON *login_service(cJSON *data, sqlite3 *db, int sock_fd) {
    int rc;
    char *sql, *error_message;
    
    uuid_t session_id;
    char session_id_str[37];

    char *username = cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring;
    char *password = cJSON_GetObjectItemCaseSensitive(data, "password")->valuestring;

    cJSON* user = get_user_by_username_service(username, db, sock_fd);
    if (user == NULL) {
        error_handler(sock_fd, "User not found", 400);
        return NULL;
    }

    if (bcrypt_checkpw(password, cJSON_GetObjectItem(user, "password")->valuestring) != 0) {
        error_handler(sock_fd, "Password doesn't match", 400);
        return NULL;
    }

    uuid_generate(session_id);
    uuid_unparse_lower(session_id, session_id_str);

    int user_id = cJSON_GetObjectItem(user, "id")->valueint;
    sql = sqlite3_mprintf(
        "INSERT INTO sessions (id, user_id, expires_at)"
        "VALUES(%Q, %d, datetime('now', '+1 month'));",
        session_id_str,
        user_id
    );
   
    rc = sqlite3_exec(db, sql, NULL, 0, &error_message);

    sqlite3_free(sql);
    uuid_clear(session_id);

    if (rc != SQLITE_OK){
        error_handler(sock_fd, error_message, 422);
        sqlite3_free(error_message);
        return NULL;
    }

    cJSON_AddStringToObject(user, "session_id", session_id_str);

    return user;
}


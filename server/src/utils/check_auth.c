#include "server.h"

bool auth_handler(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    if (headers == NULL) {
        error_handler(sock_fd, "Invalid json", 400);
        return false;
    }

    char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;
    cJSON *session = get_session(session_id, db);

    if (session == NULL) {
        error_handler(sock_fd, "Unauthorized", 401);
        return false;
    }

    cJSON_AddItemToObject(headers, "session_data", session);
    return true;
}


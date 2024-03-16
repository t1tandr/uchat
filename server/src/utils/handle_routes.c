#include "server.h"

void handle_routes(cJSON *req, sqlite3 *db, int sock_fd) {
    if (!cJSON_HasObjectItem(req, "method") || !cJSON_HasObjectItem(req, "route")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    char *route = cJSON_GetObjectItemCaseSensitive(req, "route")->valuestring;
    char *method = cJSON_GetObjectItemCaseSensitive(req, "method")->valuestring;

    if (strcmp(route, "/user") == 0) {
        if (strcmp(method, "POST") == 0) {
            cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");

            if (data == NULL) {
                error_handler(sock_fd, "Invalid json", 400);
                return;
            }

            create_user_controller(data, db, sock_fd);
        }
    }
}


#include "server.h"

void handle_routes(cJSON *req, sqlite3 *db, int sock_fd) {
    if (!cJSON_HasObjectItem(req, "method") || !cJSON_HasObjectItem(req, "route")) {
        error_handler(sock_fd, "Invalid json", 400);
        return;
    }

    char *route = cJSON_GetObjectItemCaseSensitive(req, "route")->valuestring;
    char *method = cJSON_GetObjectItemCaseSensitive(req, "method")->valuestring;

    if (strncmp(route, "/user", strlen("/user")) == 0) {
        int id;

        if (strcmp(method, "POST") == 0) {
            create_user_controller(req, db, sock_fd);
        } else if (strcmp(method, "GET") == 0 && sscanf(route, "/user/%d", &id) == 1) {
            get_user_controller(id, db, sock_fd);
        } else if (strcmp(method, "GET") == 0) {
            get_users_controller(db, sock_fd);
        }
    }
}


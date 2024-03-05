#include "server.h"

void handle_routes(cJSON *req) {
    char *route = cJSON_GetObjectItemCaseSensitive(req, "route")->valuestring;
    char *method = cJSON_GetObjectItemCaseSensitive(req, "method")->valuestring;

    if (strcmp(route, "/user") == 0) {
        if (strcmp(method, "POST") == 0) {
            cJSON *data = cJSON_GetObjectItemCaseSensitive(req, "data");
            create_user(data);
        }
    }

    // free json objects
}


#include "server.h"

void *handle_request(void *arg) {
    int sockfd = *(int*)arg;
    sqlite3 *db = database_connect(); // Maybe switch to connection pool

    while(1) {
        int length, n;

        n = recv(sockfd, &length, sizeof(length), 0);

        if (n < 0) {
            error_handler(sockfd, "Error receiving data", 400);
            continue;
        }

        int received_bytes = 0;
        char *res_str = malloc(length + 1);

        while (received_bytes < length) {
            n = recv(sockfd, res_str + received_bytes, length - received_bytes, 0);

            if (n < 0) {
                error_handler(sockfd, "Error receiving data", 400);
                continue; // how to continue outer loop without goto ??
            }

            received_bytes += n;
        }

        res_str[received_bytes] = '\0';

        cJSON *req_json = cJSON_Parse(res_str);
        
        if (!cJSON_IsObject(req_json)) {
            error_handler(sockfd, "Invalid json", 400);
            continue;
        }
        
        handle_routes(req_json, db, sockfd);

        cJSON_Delete(req_json);
        free(res_str);
    }
    sqlite3_close(db);
    close(sockfd);

    return NULL;
}


#include "server.h"

void *handle_request(void *arg) {
    int sock_fd = *(int*)arg;
    sqlite3 *db = database_connect(); // Maybe switch to connection pool

    while (1) {
        unsigned long length, n; // sooner or later this shit will blow

        n = recv(sock_fd, &length, sizeof(length), 0);

        if (n < 0) {
            error_handler(sock_fd, "Error receiving data", 400);
            continue;
        }
        if (n == 0) {
            break;
        }

        unsigned long received_bytes = 0;
        char *res_str = malloc(length + 1);

        while (received_bytes < length) {
            n = recv(sock_fd, res_str + received_bytes, length - received_bytes, 0);

            if (n < 0) {
                error_handler(sock_fd, "Error receiving data", 400);
                continue; // how to continue outer loop without goto ??
            }
            if (n == 0) {
                break;
            }

            received_bytes += n;
        }

        res_str[received_bytes] = '\0';

        cJSON *req_json = cJSON_Parse(res_str);
        
        if (!cJSON_IsObject(req_json)) {
            error_handler(sock_fd, "Invalid json", 400);
            continue;
        }
        
        handle_routes(req_json, db, sock_fd);

        cJSON_Delete(req_json);
        free(res_str);
    }

    char *session_id = find_session_by_sock(sock_fd);
    if (session_id != NULL) {
        remove_client_connection(session_id);
    }

    sqlite3_close(db);
    close(sock_fd);

    return NULL;
}


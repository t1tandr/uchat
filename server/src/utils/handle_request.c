#include "server.h"

void *handle_request(void *arg) {
    // int sock_fd = *(int*)arg;
    // int length, n;

    // n = recv(sock_fd, &length, sizeof(length), 0);

    // if (!n) {
    //     mx_printstr("do smth");
    // }

    // write(1, &length, sizeof(length));

    int sock_fd = *(int*)arg;
    sqlite3 *db = database_connect(); // Maybe switch to connection pool

    while (1) {
        int length, n;
        char buff[BUFF_SIZE];

        n = recv(sock_fd, buff, BUFF_SIZE, 0);

        if (n < 0) {
            mx_printstr("do smth");
        }

        length = atoi(buff);

        int received_bytes = 0;
        char *res_str = malloc(length + 1);

        while (received_bytes < length) {
            n = recv(sock_fd, res_str + received_bytes, length - received_bytes, 0);

            if (n < 0) {
                mx_printstr("do smth");
                break;
            }

            received_bytes += n;
        }

        res_str[received_bytes] = '\0';

        cJSON *req_json = cJSON_Parse(res_str); 
        handle_routes(req_json, db);
    }

    sqlite3_close(db);
    close(sock_fd);
    return NULL;
}


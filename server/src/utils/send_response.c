#include "server.h"

void send_response(int sock_fd, cJSON *data, int status) {
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", status);

    if (data != NULL) {
        cJSON_AddItemToObject(res, "data", data);
    }

    char *res_str = cJSON_Print(res);
    send(sock_fd, res_str, strlen(res_str), 0);

    cJSON_Delete(res);
    cJSON_free(res_str);
}


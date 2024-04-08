#include "server.h"

void error_handler(int sock_fd, char* message, int status) {
    cJSON *err_res = cJSON_CreateObject();
    cJSON_AddNumberToObject(err_res, "status", status);
    cJSON_AddStringToObject(err_res, "error", message);
    char *err_json_str = cJSON_Print(err_res);

    send(sock_fd, err_json_str, strlen(err_json_str), 0);

    cJSON_Delete(err_res);
    cJSON_free(err_json_str);
}


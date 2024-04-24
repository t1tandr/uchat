#include "server.h"

void error_handler(int sock_fd, char* message, int status) {
    cJSON *err_res = cJSON_CreateObject();
    cJSON_AddNumberToObject(err_res, "status", status);
    cJSON_AddStringToObject(err_res, "error", message);
    cJSON_AddStringToObject(err_res, "type", "regular");

    char *err_json_str = cJSON_Print(err_res);
    unsigned long length = strlen(err_json_str);
    long nbytes = 0;
     
    nbytes = send(sock_fd, &length, sizeof(length), MSG_NOSIGNAL);

    if(nbytes > -1) {
        send(sock_fd, err_json_str, length, MSG_NOSIGNAL);
    }

    cJSON_Delete(err_res);
    cJSON_free(err_json_str);
}


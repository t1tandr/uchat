#include "server.h"

void send_response(int sock_fd, cJSON *data, int status) {
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", status);

    if (data != NULL) {
        cJSON_AddItemToObject(res, "data", data);
    }

    char *res_str = cJSON_Print(res);
    int length = strlen(res_str);
    int nbytes = 0;

    nbytes = send(sock_fd, &length, sizeof(length), MSG_NOSIGNAL);
    
    if(nbytes > -1) {
        send(sock_fd, res_str, length, MSG_NOSIGNAL);
    }

    cJSON_Delete(res);
    cJSON_free(res_str);
}


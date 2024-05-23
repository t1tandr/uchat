#include "uchat.h"

cJSON* recv_response(int sockfd) {   
    cJSON* response = NULL;
    char* res_str = NULL;
    unsigned long length = 0;
    long nbytes = 0;

    nbytes = recv(sockfd, &length, sizeof(length), 0);

    if (nbytes < 0) {
        return NULL;
    }

    res_str = (char *)malloc(length + 1);

    nbytes = recv(sockfd, res_str, length, MSG_WAITALL);

    if (nbytes < 0) {
        free(res_str);
        return NULL;
    }

    res_str[nbytes] = '\0';
    response = cJSON_Parse(res_str);
    free(res_str);

    return response;
}


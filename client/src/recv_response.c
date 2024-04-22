#include "uchat.h"

cJSON* recv_response(int sockfd) {
    cJSON* response = NULL;
    char* res_str = NULL;
    int length = 0;
    int nbytes = 0;
    int received_bytes = 0;

    nbytes = recv(sockfd, &length, sizeof(length), 0);

    if (nbytes < 0) {
        return NULL;;
    }

    res_str = (char *)malloc(length + 1);

    while (received_bytes < length) {
        nbytes = recv(sockfd, res_str + received_bytes, length - received_bytes, 0);

        if (nbytes < 0) {
            free(res_str);
            return NULL;
        }
        received_bytes += nbytes;
    }
    res_str[received_bytes] = '\0';
    response = cJSON_Parse(res_str);
    free(res_str);

    return response;
}


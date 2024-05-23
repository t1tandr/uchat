#include "uchat.h"

int send_request(int sockfd, cJSON *request) {
    char* req_str = NULL;
    unsigned long length = 0;
    long nbytes = 0;

    req_str = cJSON_PrintUnformatted(request);
    cJSON_Delete(request);

    if (req_str == NULL) {
        return -1;
    }

    length = strlen(req_str);

    nbytes = send(sockfd, &length, sizeof(length), 0);
    
    if (nbytes != -1) {
        nbytes = send(sockfd, req_str, length, 0);
        free(req_str);

        if (nbytes != -1) {
            return REQUEST_SUCCESS;
        }
    }

    return -1;
}


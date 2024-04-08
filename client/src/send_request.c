#include "uchat.h"

cJSON* send_request(int sockfd, cJSON *request) {
    char* request_str = cJSON_Print(request);
    char  response_str[1024];
    int   nbytes = 0;

    memset(response_str, '\0', sizeof(response_str));

    nbytes = send(sockfd, request_str, strlen(request_str), 0);
    
    free(request_str);
    if(nbytes > 0) {
        nbytes = recv(sockfd, response_str, sizeof(response_str) - 1, 0);

        if(nbytes > 0) {
            response_str[nbytes] = '\0';
            return cJSON_Parse(response_str);
        }
    }

    return NULL;
}


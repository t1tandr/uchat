#include "uchat.h"

cJSON* send_request(int sockfd, cJSON *request) {
    cJSON* res_json = NULL;
    char* req_str = NULL;
    char* res_str = NULL;
    int req_length = 0;
    int res_length = 0;
    int nbytes = 0;

    req_str = cJSON_PrintUnformatted(request);
    if(req_str == NULL) {
        return NULL;
    }

    req_length = strlen(req_str);

    nbytes = send(sockfd, &req_length, sizeof(req_length), 0);
    
    if(nbytes != -1) {
        nbytes = send(sockfd, req_str, req_length, 0);

        if(nbytes != -1) {
            nbytes = recv(sockfd, &res_length, sizeof(res_length), 0);

            if (nbytes < 0) {
                return NULL;;
            }

            int received_bytes = 0;
            res_str = (char *)malloc(res_length + 1);

            while(received_bytes < res_length) {
                nbytes = recv(sockfd, res_str + received_bytes, res_length - received_bytes, 0);

                if (nbytes < 0) {
                    return NULL;
                }
                received_bytes += nbytes;
            }
            res_str[received_bytes] = '\0';
            res_json = cJSON_Parse(res_str);

            free(req_str);
            free(res_str);

            return res_json;
        }
    }

    return NULL;
}


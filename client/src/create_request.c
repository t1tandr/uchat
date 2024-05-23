#include "uchat.h"

cJSON* create_request(const char* method, const char* route, cJSON* data, cJSON* headers) {
    cJSON* request = cJSON_CreateObject();

    if (request != NULL && data != NULL && headers != NULL) {
        cJSON_AddItemToObject(request, "headers", headers);
        cJSON_AddStringToObject(request, "method", method);
        cJSON_AddStringToObject(request, "route",  route);
        cJSON_AddItemToObject(request, "data", data);

        return request;
    }

    return NULL;
}


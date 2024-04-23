#include "server.h"

cJSON *extract_query_params(char *route) {
    char *start = strchr(route, '?');
    if (start == NULL) return NULL;
    start += 1;

    char **param_pairs = mx_strsplit(start, '&');
    if (param_pairs[0] == NULL) return NULL;

    cJSON *query_params = cJSON_CreateObject();

    for (int i = 0; param_pairs[i] != NULL; i++) {
        char *pair = param_pairs[i];
        if (strchr(pair, '=') == NULL) continue;

        char **key_value = mx_strsplit(pair, '=');
        if (key_value == NULL) continue;

        cJSON_AddStringToObject(query_params, key_value[0], key_value[1]);

        mx_del_strarr(&key_value);
    }

    mx_del_strarr(&param_pairs);

    return query_params;
}

void query_params_handler(cJSON *req, char *route) {
    cJSON *query_params = extract_query_params(route);
    if (query_params == NULL) return;

    cJSON *headers = cJSON_GetObjectItem(req, "headers");
    cJSON_AddItemToObject(headers, "query_params", query_params);
}


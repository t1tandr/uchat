#include "server.h"

void create_user(cJSON *data) {
    char *username = cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring;
    mx_printstr(username);
}


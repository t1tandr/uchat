#include "uchat.h"

t_user* get_user_from_json(cJSON* json) {
    if(json == NULL || cJSON_HasObjectItem(json, "data")) {
        return NULL;
    }

    t_user* user = (t_user *)malloc(sizeof(t_user));
    cJSON* data = cJSON_GetObjectItemCaseSensitive(json, "data");

    user->id = cJSON_GetObjectItem(data, "id")->valueint;
    user->username = strdup(cJSON_GetObjectItem(data, "username")->valuestring);
    user->name = strdup(cJSON_GetObjectItem(data, "name")->valuestring);
    user->bio = strdup(cJSON_GetObjectItem(data, "bio")->valuestring);

    return user;
}



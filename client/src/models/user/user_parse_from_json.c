#include "uchat.h"

t_user* user_parse_from_json(cJSON* json) {
    if (json == NULL) {
        return NULL;
    }

    t_user* user = user_init();

    user->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    user->username = strdup(cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring);
    user->name = strdup(cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);

    if (cJSON_HasObjectItem(json, "bio")) {
        user->bio = strdup(cJSON_GetObjectItemCaseSensitive(json, "bio")->valuestring);
    }

    return user;
}


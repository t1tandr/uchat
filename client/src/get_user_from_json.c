#include "uchat.h"

t_user* get_user_from_json(cJSON* json) {
    if (json == NULL) {
        return NULL;
    }

    t_user* user = (t_user *)malloc(sizeof(t_user));

    user->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    user->username = strdup(cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring);
    user->name = strdup(cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
    // user->bio = strdup(cJSON_GetObjectItemCaseSensitive(json, "bio")->valuestring);

    return user;
}


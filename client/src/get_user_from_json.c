#include "uchat.h"

t_user* get_user_from_json(cJSON* json) {
    if(json == NULL) {
        return NULL;
    }

    t_user* user = (t_user *)malloc(sizeof(t_user));
    user->id = cJSON_GetObjectItemCaseSensitive(json, "user_id")->valueint;
    user->session = strdup(cJSON_GetObjectItemCaseSensitive(json, "session_id")->valuestring);

    /*
    user->username = strdup(cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring);
    user->name = strdup(cJSON_GetObjectItemCaseSensitive(data, "name")->valuestring);
    user->bio = strdup(cJSON_GetObjectItemCaseSensitive(data, "bio")->valuestring);
    */
   
    return user;
}



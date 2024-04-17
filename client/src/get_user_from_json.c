#include "uchat.h"

t_user* get_user_from_json(cJSON* json) {
    if(json == NULL || !cJSON_HasObjectItem(json, "data")) {
        return NULL;
    }

    t_user* user = (t_user *)malloc(sizeof(t_user));
    cJSON* data = cJSON_GetObjectItemCaseSensitive(json, "data");

    user->id = cJSON_GetObjectItemCaseSensitive(data, "user_id")->valueint;
    user->session = strdup(cJSON_GetObjectItemCaseSensitive(data, "session_id")->valuestring);
    
    /*
    user->username = strdup(cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring);
    user->name = strdup(cJSON_GetObjectItemCaseSensitive(data, "name")->valuestring);
    user->bio = strdup(cJSON_GetObjectItemCaseSensitive(data, "bio")->valuestring);

    */

    return user;
}



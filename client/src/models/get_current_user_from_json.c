#include "uchat.h"

t_current_user* get_current_user_from_json(cJSON* json) {
    if(json == NULL) {
        return NULL;
    }

    t_current_user* user = (t_current_user *)malloc(sizeof(t_current_user));
    
    user->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    user->session = strdup(cJSON_GetObjectItemCaseSensitive(json, "session_id")->valuestring);
    user->username = strdup(cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring);
    user->name = strdup(cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
    
    if (cJSON_HasObjectItem(json, "bio")) {
        user->bio = strdup(cJSON_GetObjectItemCaseSensitive(json, "bio")->valuestring);
    }
   
    return user;
}



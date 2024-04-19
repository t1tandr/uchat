#include "uchat.h"

t_user* get_user_from_json(cJSON* json) {
    if(json == NULL) {
        return NULL;
    }
    cJSON* data = cJSON_GetObjectItemCaseSensitive(json, "data");
    if (data == NULL) {
        cJSON_Delete(json);
        return NULL;
    }

    t_user* user = (t_user *)malloc(sizeof(t_user));
    mx_printstr("9");
    user->id = cJSON_GetObjectItemCaseSensitive(data, "user_id")->valueint;
    mx_printstr("11");
    user->session = strdup(cJSON_GetObjectItemCaseSensitive(data, "session_id")->valuestring);
    mx_printstr("13");
    /*
    user->username = strdup(cJSON_GetObjectItemCaseSensitive(data, "username")->valuestring);
    user->name = strdup(cJSON_GetObjectItemCaseSensitive(data, "name")->valuestring);
    user->bio = strdup(cJSON_GetObjectItemCaseSensitive(data, "bio")->valuestring);

    */
    cJSON_Delete(json);
    return user;
}



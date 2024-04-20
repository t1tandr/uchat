#include "uchat.h"

t_chat* get_chat_from_json(cJSON* json) {
    if(json == NULL) {
        return NULL;
    }

    t_chat* chat = (t_chat *)malloc(sizeof(t_chat));

    chat->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    chat->name = strdup(cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
    // user->bio = strdup(cJSON_GetObjectItemCaseSensitive(json, "bio")->valuestring);

    return chat;
}


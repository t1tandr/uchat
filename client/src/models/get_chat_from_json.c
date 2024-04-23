#include "uchat.h"

t_chat* get_chat_from_json(cJSON* json) {
    if (json == NULL) {
        return NULL;
    }

    t_chat* chat = (t_chat *)malloc(sizeof(t_chat));

    chat->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    chat->name = strdup(cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
    chat->created_at = strdup(cJSON_GetObjectItemCaseSensitive(json, "created_at")->valuestring);
    chat->members = NULL;
    chat->last_message = NULL;
    chat->img = NULL;

    return chat;
}


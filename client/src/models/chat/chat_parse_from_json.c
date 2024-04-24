#include "uchat.h"

t_chat* chat_parse_from_json(cJSON* json) {
    if (json == NULL) {
        return NULL;
    }

    t_chat* chat = chat_init();

    chat->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    chat->name = strdup(cJSON_GetObjectItemCaseSensitive(json, "name")->valuestring);
    chat->created_at = strdup(cJSON_GetObjectItemCaseSensitive(json, "created_at")->valuestring);

    return chat;
}


#include "uchat.h"

t_message* get_message_from_json(cJSON* json) {
    if (json == NULL) {
        return NULL;
    }

    t_message* msg = (t_message *)malloc(sizeof(t_message));

    msg->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    msg->chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;
    msg->user_id = cJSON_GetObjectItemCaseSensitive(json, "user_id")->valueint;
    msg->content = strdup(cJSON_GetObjectItemCaseSensitive(json, "content")->valuestring);
    // user->bio = strdup(cJSON_GetObjectItemCaseSensitive(json, "bio")->valuestring);
    msg->time = strdup(cJSON_GetObjectItemCaseSensitive(json, "created_at")->valuestring);

    return msg;
}


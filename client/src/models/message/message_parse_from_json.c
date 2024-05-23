#include "uchat.h"

static int get_message_type(const char* type) {
    if (strcmp(type, "text") == 0) {
        return MSG_TYPE_TXT;
    }
    if (strcmp(type, "photo") == 0) {
        return MSG_TYPE_IMG;
    }
    if (strcmp(type, "video") == 0) {
        return MSG_TYPE_VID;
    }
    if (strcmp(type, "audio") == 0) {
        return MSG_TYPE_AUD;
    }

    return MSG_TYPE_UNKNOWN;
}

t_message* message_parse_from_json(cJSON* json) {
    if (json == NULL) {
        return NULL;
    }

    t_message* msg = message_init();

    msg->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    msg->chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;
    msg->user_id = cJSON_GetObjectItemCaseSensitive(json, "user_id")->valueint;
    msg->content = strdup(cJSON_GetObjectItemCaseSensitive(json, "content")->valuestring);
    msg->type = get_message_type(cJSON_GetObjectItemCaseSensitive(json, "type")->valuestring);
    msg->time = strdup(cJSON_GetObjectItemCaseSensitive(json, "created_at")->valuestring);
    msg->author = strdup(cJSON_GetObjectItemCaseSensitive(json, "username")->valuestring);

    return msg;
}


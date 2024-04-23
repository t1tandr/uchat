#include "uchat.h"

static int get_member_role(const char* role) {
    if (strcmp(role, "USER") == 0) {
        return ROLE_USER;
    }
    if (strcmp(role, "ADMIN") == 0) {
        return ROLE_ADMIN;
    }

    return ROLE_USER;
}

t_chat_member* chat_member_parse_from_json(cJSON* json) {
    if (json == NULL) {
        return NULL;
    }

    t_chat_member* member = chat_member_init();

    member->id = cJSON_GetObjectItemCaseSensitive(json, "id")->valueint;
    member->chat_id = cJSON_GetObjectItemCaseSensitive(json, "chat_id")->valueint;
    member->user_id = cJSON_GetObjectItemCaseSensitive(json, "user_id")->valueint;
    member->role = get_member_role(cJSON_GetObjectItemCaseSensitive(json, "role")->valuestring);

    return member;
}


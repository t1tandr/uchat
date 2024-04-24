#ifndef CHAT_MEMBER_H
#define CHAT_MEMBER_H

#include <cJSON.h>

/*
 * CHAT MEMBER UTILS
*/

enum {
    ROLE_USER,
    ROLE_ADMIN
};

typedef struct s_chat_member t_chat_member;

struct s_chat_member {
    int id;
    int chat_id;
    int user_id;
    int role;
    const char* username;
};

t_chat_member* chat_member_init(void);

t_chat_member* chat_member_parse_from_json(cJSON* json);

t_list* chat_member_parse_from_json_arr(cJSON* arr);

char* chat_member_get_username_by_id(int id);

#endif
#ifndef CHAT_H
#define CHAT_H

#include <cJSON.h>
#include "libmx.h"
#include "message.h"

/*
 * CHAT UTILS
*/

typedef struct s_chat t_chat;

struct s_chat {
    int id;
    const char* name;
    const char* img;
    const char* created_at;
    t_list* members;
    t_list* messages;
    t_message* last_message;
};

t_chat* chat_init(void);

t_chat* chat_parse_from_json(cJSON* json);

t_list* chat_parse_from_json_arr(cJSON* arr);

void chat_add_members(t_chat* chat);

#endif

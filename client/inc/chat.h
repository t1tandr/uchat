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
    int size;
    t_list* members;
    t_message* last_message;
};

t_chat* get_chat_from_json(cJSON* json);

t_list* get_chats_from_json_arr(cJSON* arr);

#endif

#ifndef CHAT_H
#define CHAT_H

#include <cJSON.h>
#include "libmx.h"

/*
 * CHAT UTILS
*/

typedef struct s_chat t_chat;

struct s_chat {
    int id;
    const char* name;
    GFile* img;
    int size;
    t_list* members;
};

t_chat* get_chat_from_json(cJSON* json);

t_list* get_chats_from_json_arr(cJSON* arr);

#endif

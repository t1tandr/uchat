#ifndef MESSAGE_H
#define MESSAGE_H

#include <cJSON.h>

/*
 * MESSAGE UTILS
*/

typedef struct s_message t_message;

struct s_message {
    int id;
    int user_id;
    int chat_id;
    const char* time;
    const char* message;
};

t_message* get_message_from_json(cJSON* json);

#endif


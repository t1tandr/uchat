#ifndef MESSAGE_H
#define MESSAGE_H

#include <cJSON.h>

/*
 * MESSAGE UTILS
*/

enum {
    MSG_TYPE_TXT,
    MSG_TYPE_IMG
};

typedef struct s_message t_message;

struct s_message {
    int id;
    int user_id;
    int chat_id;
    int type;
    const char* author;
    const char* time;
    const char* content;
};

t_message* get_message_from_json(cJSON* json);

#endif


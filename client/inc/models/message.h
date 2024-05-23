#ifndef MESSAGE_H
#define MESSAGE_H

#include <cJSON.h>
#include "libmx.h"

/*
 * MESSAGE UTILS
*/

enum {
    MSG_TYPE_TXT,
    MSG_TYPE_IMG,
    MSG_TYPE_VID,
    MSG_TYPE_AUD,
    MSG_TYPE_UNKNOWN
};

typedef struct s_message t_message;

struct s_message {
    int id;
    int user_id;
    int chat_id;
    int type;
    const char* content;
    const char* author;
    const char* time;
};

t_message* message_init(void);

t_message* message_parse_from_json(cJSON* json);

t_list* message_parse_from_json_arr(cJSON* arr);

#endif


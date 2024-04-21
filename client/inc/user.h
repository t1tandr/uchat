#ifndef USER_H
#define USER_H

#include <cJSON.h>
#include "libmx.h"

/*
 * CURRENT USER UTILS
*/

typedef struct s_current_user t_current_user;

struct s_current_user {
    int id;
    const char* username;
    const char* name;
    const char* bio;
    const char* session;
    // t_chat* current_chat;
    t_list* chats;
};

t_current_user* get_current_user_from_json(cJSON* json);

/*
 * CURRENT USER UTILS
*/

typedef struct s_user t_user;

struct s_user {
    int id;
    const char* username;
    const char* name;
    const char* bio;
};

t_user* get_user_from_json(cJSON* json);

/*
 * MESSAGE UTILS (?)
*/

typedef struct s_message t_message;

struct s_message {
    int user_id;
    const char* message;
};

t_message* get_message_from_json(cJSON* json);

#endif


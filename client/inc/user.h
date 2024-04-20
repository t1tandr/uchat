#ifndef USER_H
#define USER_H

#include "uchat.h"

/*
 * CHAT UTILS
*/

typedef struct s_chat {
    int id;
    const char* name;
    t_list* members;
} t_chat;

t_chat* get_chat_from_json(cJSON* json);

/*
 * USER UTILS
*/

typedef struct s_user {
    int id;
    const char* username;
    const char* name;
    const char* bio;
    const char* session;
    t_chat* current_chat;
    t_list* chats;
} t_user;

t_user* get_user_from_json(cJSON* json);

/*
 * MESSAGE UTILS (?)
*/

typedef struct s_message {
    int user_id;
    const char* message;
} t_message;

t_message* get_message_from_json(cJSON* json);

#endif
#include "uchat.h"

t_message* message_init(void) {
   t_message* msg = (t_message *)malloc(sizeof(t_message));

    if (msg == NULL) {
        return NULL;
    }

    msg->id = 0;
    msg->user_id = 0;
    msg->chat_id = 0;
    msg->type = MSG_TYPE_UNKNOWN;
    msg->content = NULL;
    msg->time = NULL;
    msg->author = NULL;

    return msg;
}
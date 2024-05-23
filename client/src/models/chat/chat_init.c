#include "uchat.h"

t_chat* chat_init(void) {
    t_chat* chat = (t_chat *)malloc(sizeof(t_chat));

    if (chat == NULL) {
        return NULL;
    }

    chat->id = 0;
    chat->name = NULL;
    chat->img = NULL;
    chat->created_at = NULL;
    chat->members = NULL;
    chat->messages = NULL;
    chat->last_message = NULL;

    return chat;
}


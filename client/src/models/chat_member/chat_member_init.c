#include "uchat.h"

t_chat_member* chat_member_init(void) {
    t_chat_member* member = (t_chat_member *)malloc(sizeof(t_chat_member));

    if (member == NULL) {
        return NULL;
    }

    member->id = 0;
    member->chat_id = 0;
    member->user_id = 0;
    member->role = ROLE_USER;

    return member;
}


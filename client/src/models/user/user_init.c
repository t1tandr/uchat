#include "uchat.h"

t_user* user_init(void) {
    t_user* user = (t_user *)malloc(sizeof(t_user));

    if (user == NULL) {
        return NULL;
    }

    user->id = 0;
    user->username = NULL;
    user->name = NULL;
    user->bio = NULL;

    return user;
}


#include "uchat.h"

static const char* files[] = {
    "resources/ui/login.ui",
    "resources/ui/homepage.ui",
    "resources/ui/chatnewdialog.ui",
    "resources/ui/settings.ui",
    "resources/ui/registerdialog.ui",
    "resources/ui/savedialog.ui",
    NULL
};

t_uchat* uchat_create(int sockfd, GtkApplication* app) {
    t_uchat* uchat = uchat_init();

    uchat->servsock = sockfd;
    uchat->builder = builder_create(files);
    uchat->app = app;
    uchat->responses = g_async_queue_new();

    char* session = mx_file_to_str("session.json");
    if(session != NULL) {
        if (strlen(session) != 0) {
            uchat->user = get_current_user_from_json(cJSON_Parse(session));
        }
    }
    else if (session == NULL && errno != ENOENT) {
        handle_error(SESSION_ERROR, strerror(errno));
    }

    return uchat;
}


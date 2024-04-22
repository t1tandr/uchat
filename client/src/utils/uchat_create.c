#include "uchat.h"

static const char* files[] = {
    "resources/ui/login.ui",
    "resources/ui/homepage.ui",
    "resources/ui/chatnewdialog.ui",
    NULL
};

t_uchat* uchat_create(int sockfd, GtkApplication* app) {
    t_uchat* uchat = (t_uchat *)malloc(sizeof(t_uchat));

    uchat->servsock = sockfd;
    uchat->builder = builder_create(files);
    uchat->app = app;
    uchat->user = NULL;
    uchat->response = NULL;
    uchat->request_sent = false;

    return uchat;
}


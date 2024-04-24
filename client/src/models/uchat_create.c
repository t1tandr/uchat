#include "uchat.h"

static const char* files[] = {
    "resources/ui/login.ui",
    "resources/ui/homepage.ui",
    "resources/ui/chatnewdialog.ui",
    "resources/ui/afteregisterdialog.ui",
    NULL
};

t_uchat* uchat_create(int sockfd, GtkApplication* app) {
    t_uchat* uchat = (t_uchat *)malloc(sizeof(t_uchat));

    uchat->servsock = sockfd;
    uchat->builder = builder_create(files);
    uchat->app = app;
    uchat->user = NULL;
    uchat->responses = g_async_queue_new();

    return uchat;
}


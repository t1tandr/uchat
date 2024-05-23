#include "uchat.h"

static const char* files[] = {
    "resources/ui/login.ui",
    "resources/ui/homepage.ui",
    "resources/ui/chatnewdialog.ui",
    "resources/ui/afteregisterdialog.ui",
    NULL
};

t_uchat* uchat_init(void) {
    t_uchat* uchat = (t_uchat *)malloc(sizeof(t_uchat));

    uchat->servsock = 0;
    uchat->builder = NULL;
    uchat->app = NULL;
    uchat->user = NULL;
    uchat->responses = NULL;

    return uchat;
}


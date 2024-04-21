#ifndef UCHAT_CLIENT_H
#define UCHAT_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <time.h>

#include <gtk/gtk.h>
#include <cJSON.h>
#include "libmx.h"

#include "password.h"
#include "utils.h"
#include "user.h"
#include "chat.h"

#include "templates/uchatchatbox.h"
#include "templates/uchatmessagebox.h"
#include "templates/uchatuserbox.h"
#include "templates/uchatavatarbox.h"

#define METHOD_GET      "GET"
#define METHOD_POST     "POST"
#define METHOD_PUT      "PUT"
#define METHOD_DELETE   "DELETE"

#define USAGE_ERROR "usage: uchat <server-ip> <server-port>"

typedef struct s_uchat {
    int servsock;
    const char* session;
    GtkBuilder* builder;
    GtkApplication* app;
    t_current_user* user;
} t_uchat;

int connect_to_server(const char* ip, const char* port);

cJSON* create_request(const char* method, const char* route, cJSON* data, cJSON* headers);
cJSON* send_request(int sockfd, cJSON* request);

#endif


#ifndef UCHAT_CLIENT_H
#define UCHAT_CLIENT_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include <gtk/gtk.h>
#include <cJSON.h>
#include "libmx.h"

#include "password.h"
#include "utils.h"
#include "user.h"
#include "chat.h"
#include "message.h"

#include "templates/chatbox.h"
#include "templates/textmessage.h"
#include "templates/userbox.h"
#include "templates/avatarbox.h"
#include "templates/messagebox.h"

#define METHOD_GET      "GET"
#define METHOD_POST     "POST"
#define METHOD_PUT      "PUT"
#define METHOD_DELETE   "DELETE"

#define USAGE_ERROR "usage: uchat <server-ip> <server-port>"

typedef struct s_uchat {
    int servsock;
    GtkBuilder* builder;
    GtkApplication* app;
    t_current_user* user;
} t_uchat;

extern t_uchat* uchat;

t_uchat* uchat_create(int sockfd, GtkApplication* app);

int connect_to_server(const char* ip, const char* port);

cJSON* create_request(const char* method, const char* route, cJSON* data, cJSON* headers);
cJSON* send_request(int sockfd, cJSON* request);
cJSON* recv_response(int sockfd);

#endif


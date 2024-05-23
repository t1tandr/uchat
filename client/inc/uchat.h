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
#include <sys/types.h>
#include <sys/stat.h>

#include <gtk/gtk.h>
#include <cJSON.h>
#include "libmx.h"

#include "password.h"
#include "utils.h"
#include "error.h"

#include "models/user.h"
#include "models/chat.h"
#include "models/chat_member.h"
#include "models/message.h"

#include "templates/chatbox.h"
#include "templates/textmessage.h"
#include "templates/userbox.h"
#include "templates/avatarbox.h"
#include "templates/messagebox.h"
#include "templates/imagemessage.h"

#define METHOD_GET      "GET"
#define METHOD_POST     "POST"
#define METHOD_PUT      "PUT"
#define METHOD_DELETE   "DELETE"

#define REQUEST_SUCCESS 0

typedef struct s_uchat {
    int servsock;
    GtkBuilder* builder;
    GtkApplication* app;
    t_current_user* user;
    GAsyncQueue* responses;
} t_uchat;

extern t_uchat* uchat;

t_uchat* uchat_init(void);

t_uchat* uchat_create(int sockfd, GtkApplication* app);

int connect_to_server(const char* ip, const char* port);

cJSON* create_request(const char* method, const char* route, cJSON* data, cJSON* headers);
int send_request(int sockfd, cJSON* request);
cJSON* recv_response(int sockfd);

void init_listener_thread(void);

unsigned char* file_to_bytes(const char *file_name, long *file_size);
void bytes_to_file(const unsigned char *bytes, unsigned long size, const char *output_file_name);

void handle_message_response(cJSON* json);

#endif


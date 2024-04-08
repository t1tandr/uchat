#ifndef UCHAT_CLIENT_H
#define UCHAT_CLIENT_H

#include <gtk/gtk.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include <cJSON.h>

#include "libmx.h"
#include "password.h"

#define METHOD_GET      "GET"
#define METHOD_POST     "POST"
#define METHOD_PUT      "PUT"
#define METHOD_DELETE   "DELETE"

#define USAGE_ERROR "usage: uchat <server-ip> <server-port>"

void handle_error(const char* message);

int connect_to_server(const char* ip, const char* port);

cJSON* create_request(const char* method, const char* route, cJSON* data);
cJSON* send_request(int sockfd, cJSON* request);

bool widget_has_css_class(GtkWidget* self, const char* class);

void app_activate_cb(GtkApplication* app);

void connect_css(const char* path);
void add_icon_theme(const char* path);

#endif


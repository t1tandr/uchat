#ifndef UCHAT_CLIENT_H
#define UCHAT_CLIENT_H

#include <gtk/gtk.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cJSON.h>

#define METHOD_GET      "GET"
#define METHOD_POST     "POST"
#define METHOD_PUT      "PUT"
#define METHOD_DELETE   "DELETE"

int connect_to_server(const char* ip, const char* port);

cJSON* create_request(const char* method, const char* route, cJSON* data);
char* send_request(int sockfd, cJSON *request);

#endif


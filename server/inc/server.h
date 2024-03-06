#pragma once

#include "libmx.h"

#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>

#include <pthread.h>

#include <cJSON.h>
#include "sqlite3.h"

#define BUFF_SIZE 1024

// Controllers

// User
void create_user_controller(cJSON *data, sqlite3 *db);


// Utils
void *handle_request(void *arg);
void handle_routes(cJSON *req, sqlite3 *db);
int start_server_socket(int port);
void accept_clients(int sock_fd);
void init_database();
sqlite3 *database_connect();

bool is_file_exists(char *filename);

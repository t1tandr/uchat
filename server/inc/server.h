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

// - User
void create_user_controller(cJSON *req, sqlite3 *db, int sock_fd);
void get_users_controller(sqlite3 *db, int sock_fd);
void get_user_controller(int user_id, sqlite3 *db, int sock_fd);

// Services

// - User
cJSON *get_users_service(sqlite3 *db, int sock_fd);
cJSON *get_user_by_id_service(int user_id, sqlite3 *db, int sock_fd);
cJSON *get_user_by_username_service(char* username, sqlite3 *db, int sock_fd);
cJSON *create_user_service(char *username, char* password, sqlite3 *db, int sock_fd);

// Utils
void *handle_request(void *arg);
void handle_routes(cJSON *req, sqlite3 *db, int sock_fd);
int start_server_socket(int port);
void accept_clients(int sock_fd);
void init_database();
sqlite3 *database_connect();
void error_handler(int sock_fd, char* message, int status);

bool is_file_exists(char *filename);

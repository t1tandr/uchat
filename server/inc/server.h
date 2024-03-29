#pragma once

#include "libmx.h"

#include <errno.h>

#include <sys/resource.h>
#include <sys/stat.h>

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
void update_user_controller(int user_id, cJSON *req, sqlite3 *db, int sock_fd);
void delete_user_controller(int user_id, sqlite3 *db, int sock_fd);

// - Login
void login_controller(cJSON *req, sqlite3 *db, int sock_fd);

// Services

// - User
cJSON *get_users_service(sqlite3 *db, int sock_fd);
cJSON *get_user_by_id_service(int user_id, sqlite3 *db, int sock_fd);
cJSON *get_user_by_username_service(char* username, sqlite3 *db, int sock_fd);
cJSON *create_user_service(cJSON *data, sqlite3 *db, int sock_fd);
cJSON *update_user_by_id_service(int user_id, cJSON *data, sqlite3 *db, int sock_fd);
cJSON *delete_user_by_id_service(int user_id, sqlite3 *db, int sock_fd);

// - Login
cJSON *login_service(cJSON *data, sqlite3 *db, int sock_fd);

// Utils
void *handle_request(void *arg);
void handle_routes(cJSON *req, sqlite3 *db, int sock_fd);
int start_server_socket(int port);
void accept_clients(int sock_fd);
void init_database();
sqlite3 *database_connect();
void error_handler(int sock_fd, char* message, int status);
void send_response(int sock_fd, cJSON *data, int status);
void start_daemon_process();

bool is_file_exists(char *filename);

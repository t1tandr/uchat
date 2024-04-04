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
#include <bcrypt.h>
#include <uuid/uuid.h>

#define BUFF_SIZE 1024

// Controllers

// - User
void create_user_controller(cJSON *req, sqlite3 *db, int sock_fd);
void get_users_controller(sqlite3 *db, int sock_fd);
void get_user_controller(int user_id, sqlite3 *db, int sock_fd);
void update_user_controller(int user_id, cJSON *req, sqlite3 *db, int sock_fd);
void delete_user_controller(int user_id, cJSON *req, sqlite3 *db, int sock_fd);

// - Login
void login_controller(cJSON *req, sqlite3 *db, int sock_fd);

// - Logout
void logout_controller(cJSON *req, sqlite3 *db, int sock_fd);

// - Chat
void create_chat_controller(cJSON *req, sqlite3 *db, int sock_fd);
void get_chats_controller(cJSON *req, sqlite3 *db, int sock_fd);
void update_chat_controller(int chat_id, cJSON *req, sqlite3 *db, int sock_fd);
void delete_chat_controller(int chat_id, cJSON *req, sqlite3 *db, int sock_fd);

// - Message
void create_message_controller(cJSON *req, sqlite3 *db, int sock_fd);

// Services

// - User
cJSON *get_users_service(sqlite3 *db, int sock_fd);
cJSON *get_user_by_id_service(int user_id, sqlite3 *db, int sock_fd);
cJSON *get_user_by_username_service(char* username, sqlite3 *db, int sock_fd);
cJSON *create_user_service(cJSON *data, sqlite3 *db, int sock_fd);
cJSON *update_user_by_id_service(int user_id, cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd);
cJSON *delete_user_by_id_service(int user_id, cJSON *headers, sqlite3 *db, int sock_fd);

// - Login
cJSON *login_service(cJSON *data, sqlite3 *db, int sock_fd);

// - Logout
int logout_service(cJSON *data, sqlite3 *db, int sock_fd);

// - Chat
cJSON *create_chat_service(cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd);
cJSON *get_chats_service(cJSON *headers, sqlite3 *db, int sock_fd);
cJSON *update_chat_by_id_service(int chat_id, cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd);
cJSON *delete_chat_by_id_service(int chat_id, cJSON *headers, sqlite3 *db, int sock_fd);

// - Message
cJSON *create_message_service(cJSON *data, cJSON *headers, sqlite3 *db, int sock_fd);

// - ChatMember
cJSON *get_chat_members_service(int chat_id, sqlite3 *db, int sock_fd);

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
bool session_exists(char *session_id, int user_id, sqlite3 *db);
cJSON *get_session(char *session_id, sqlite3 *db);

bool is_file_exists(char *filename);
cJSON *stmt_to_user_json(sqlite3_stmt *stmt);
cJSON *stmt_to_message_json(sqlite3_stmt *stmt);
cJSON *stmt_to_chat_json(sqlite3_stmt *stmt);
bool is_user_chat_member(int user_id, cJSON *chat_members);

void send_response_message_all(int sock_fd, sqlite3 *db, cJSON *message, int status);
void add_client_connection(char *session_id, int user_id, int sock_fd);
void remove_client_connection(char *session_id);
bool is_client_saved(char *session_id, int sock_fd);

typedef struct {
    int sock_fd;
    char *session_id;
    int user_id;
} connection;

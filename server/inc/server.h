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
void create_user(cJSON *data);


// Utils
void *handle_request(void *arg);
void handle_routes(cJSON *req);

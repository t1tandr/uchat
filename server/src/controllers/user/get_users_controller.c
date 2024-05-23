#include "server.h"

void get_users_controller(sqlite3 *db, int sock_fd) {
    cJSON *users = get_users_service(db, sock_fd);

    if (users == NULL) return;

    for (int i = 0; i < cJSON_GetArraySize(users); i++) {
        cJSON *user = cJSON_GetArrayItem(users, i);
        cJSON_DeleteItemFromObject(user, "password");
    }

    send_response(sock_fd, users, 200);
}


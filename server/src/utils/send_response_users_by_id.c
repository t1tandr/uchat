#include "server.h"

extern GHashTable *clients;

void send_response_users_by_id(GHashTable *user_ids, cJSON *data, int status) {
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", status);

    if(data != NULL) {
        cJSON_AddItemToObject(res, "data", data);
    }

    char *res_str = cJSON_Print(res);
    int length = strlen(res_str);
    int nbytes = 0;

    GHashTableIter iter;
    gpointer value;

    g_hash_table_iter_init (&iter, clients);
    while(g_hash_table_iter_next (&iter, NULL, &value)) {
        connection_data *conn = (connection_data *) value;
        char *user_id = mx_itoa(conn->user_id);

        if(g_hash_table_contains(user_ids, (const void *) user_id)) {
            nbytes = send(conn->sock_fd, &length, sizeof(length), MSG_NOSIGNAL);
            
            if(nbytes > -1) {
                send(conn->sock_fd, res_str, length, MSG_NOSIGNAL);
            }
        }

        free(user_id);
    }

    g_hash_table_destroy(user_ids);
    cJSON_Delete(res);
    cJSON_free(res_str);
}


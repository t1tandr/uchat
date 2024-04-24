#include "server.h"

extern GHashTable *clients;

void send_response_users_by_id(GHashTable *user_ids, cJSON *data, int status, int src_sock_fd) {
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", status);

    if(data != NULL) {
        cJSON_AddItemToObject(res, "data", data);
    }

    cJSON_AddStringToObject(res, "type", "regular");

    char *src_res_str = cJSON_Print(res);
    unsigned long src_length = strlen(src_res_str);

    cJSON_ReplaceItemInObject(res, "type", cJSON_CreateString("socket"));

    char *res_str = cJSON_Print(res);
    unsigned long length = strlen(res_str);

    GHashTableIter iter;
    gpointer value;

    g_hash_table_iter_init (&iter, clients);
    while(g_hash_table_iter_next (&iter, NULL, &value)) {
        connection_data *conn = (connection_data *) value;
        char *user_id = mx_itoa(conn->user_id);
        long nbytes = 0;

        if(g_hash_table_contains(user_ids, (const void *) user_id)) {
            if (conn->sock_fd == src_sock_fd) {
                nbytes = send(conn->sock_fd, &src_length, sizeof(src_length), MSG_NOSIGNAL);
            
                if(nbytes > -1) {
                    send(conn->sock_fd, src_res_str, src_length, MSG_NOSIGNAL);
                }
            } else {
                nbytes = send(conn->sock_fd, &length, sizeof(length), MSG_NOSIGNAL);
            
                if(nbytes > -1) {
                    send(conn->sock_fd, res_str, length, MSG_NOSIGNAL);
                }
            } 
        }

        free(user_id);
    }

    g_hash_table_destroy(user_ids);
    cJSON_Delete(res);
    cJSON_free(res_str);
}


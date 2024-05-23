#include "server.h"

extern GHashTable *clients;

void client_connection_handler(cJSON *req, sqlite3 *db, int sock_fd) {
    cJSON *headers = cJSON_GetObjectItemCaseSensitive(req, "headers");

    if (cJSON_HasObjectItem(headers, "Authorization")) {
        char *session_id = cJSON_GetObjectItem(headers, "Authorization")->valuestring;
        cJSON *session = get_session(session_id, db);

        if (session != NULL && !is_client_saved(session_id)) {
            int user_id = cJSON_GetObjectItem(session, "user_id")->valueint;
            
            add_client_connection(session_id, user_id, sock_fd);
        }
    }
}

void add_client_connection(char *session_id, int user_id, int sock_fd) {
    connection_data *conn = (connection_data *) malloc(sizeof(conn));
    conn->sock_fd = sock_fd;
    conn->user_id = user_id;

    g_hash_table_insert(clients, mx_strdup(session_id), conn);
}

char *find_session_by_sock(int sock_fd) {
    GHashTableIter iter;
    gpointer value, key;

    g_hash_table_iter_init (&iter, clients);
    while (g_hash_table_iter_next (&iter, &key, &value)) {
        connection_data *conn = (connection_data *) value;

        if (conn->sock_fd == sock_fd) {
            return mx_strdup((char *) key);
        }
    }

    return NULL;
}

void remove_client_connection(char *session_id) {
    g_hash_table_remove(clients, session_id);
}

bool is_client_saved(char *session_id) {
    return g_hash_table_contains(clients, session_id);
}


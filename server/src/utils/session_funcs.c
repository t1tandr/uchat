#include "server.h"

extern t_list *clients;

void add_client_connection(char *session_id, int user_id, int sock_fd) {
    connection *conn = (connection *) malloc(sizeof(conn));
    conn->sock_fd = sock_fd;
    conn->user_id = user_id;
    conn->session_id = mx_strdup(session_id);

    if (clients == NULL) {
        clients = mx_create_node((void *) conn);
    } else {
        mx_push_back(&clients, (void *) conn);
    }
}

bool session_cmp(void *a, void *b) {
    connection *conn = (connection *) a;

    return strcmp(conn->session_id, (char *) b) == 0;
}

void remove_client_connection(char *session_id) {
    mx_del_node_if(&clients, (void *) session_id, session_cmp);
}

bool is_client_saved(char *session_id, int sock_fd) {
    t_list *temp = clients;

    while (temp != NULL) {
        connection *conn = (connection *) temp->data;

        if (conn->sock_fd == sock_fd && strcmp(conn->session_id, session_id) == 0) {
            return true;
        }

        temp = temp->next;
    }

    return false;
}


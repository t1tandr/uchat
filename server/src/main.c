#include "server.h"

GHashTable *clients; // correct handle in multiple threads

int main(int argc, char *argv[]) {
    int sock_fd, port;

    if (argc < 2) {
        mx_printerr("usage: uchat-server <port-number>");
        exit(EXIT_FAILURE);
    }

    start_daemon_process();

    clients = g_hash_table_new(g_str_hash, g_str_equal);

    port = atoi(argv[1]);

    init_database();
    sock_fd = start_server_socket(port);
    accept_clients(sock_fd);

    close(sock_fd);
}


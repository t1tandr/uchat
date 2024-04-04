#include "server.h"

t_list *clients = NULL; // correct handle in multiple threads && user hashmap

int main(int argc, char *argv[]) {
    // start_daemon_process();

    int sock_fd, port;

    if (argc < 2) {
        mx_printerr("usage: uchat-server <port-number>");
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    init_database();
    sock_fd = start_server_socket(port);
    accept_clients(sock_fd);

    close(sock_fd);
}


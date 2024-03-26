#include "server.h"

int main(int argc, char *argv[]) {
    int sock_fd, port;

    if (argc < 2) {
        mx_printerr("usage: <port_number>");
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    init_database();
    sock_fd = start_server_socket(port);
    accept_clients(sock_fd);

    close(sock_fd);
}


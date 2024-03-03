#include "server.h"

void *handle_request(void *arg) {
    int sock_fd = *(int*)arg;

    while (1) {
        char buffer[1024];
        int n;

        n = recv(sock_fd, buffer, 1024, 0);

        if (n == 0) {
            break;
        }

        buffer[n] = '\0';
        mx_printstr(buffer);
    }
    close(sock_fd);
    return NULL;
}

int main(int argc, char *argv[]) {
    int sock_fd, port;
    struct sockaddr_in serv_addr;

    if (argc < 2) {
        mx_printerr("usage: <port_number>");
        exit(EXIT_FAILURE);
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd < 0) {
        mx_printerr("socket failed");
        exit(EXIT_FAILURE);
    }

    port = mx_atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        mx_printerr("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(sock_fd, 10) < 0) {
        mx_printerr("listen failed");
        exit(EXIT_FAILURE);
    }


    while (1) {
        int new_sock = accept(sock_fd, NULL, NULL);

        if (new_sock < 0) {
            mx_printerr("accept failed");
            exit(EXIT_FAILURE);
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_request, (void *) &new_sock);
        pthread_detach(thread_id);
    }

    close(sock_fd);
}


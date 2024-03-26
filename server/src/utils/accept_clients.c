#include "server.h"

void accept_clients(int sock_fd) {
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
}


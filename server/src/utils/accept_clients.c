#include "server.h"

void accept_clients(int sock_fd, SSL_CTX *ctx) {
    while (1) {
        int new_sock = accept(sock_fd, NULL, NULL);

        if (new_sock < 0) {
            mx_printstr("hello14");
            mx_printerr("accept failed");
            exit(EXIT_FAILURE);
        }

        SSL *ssl = SSL_new(ctx);
        SSL_set_fd(ssl, new_sock);

        if (SSL_accept(ssl) == -1) {
            mx_printstr("hello15");
            perror(strerror(errno));
            close(new_sock);
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_request, (void *) ssl);
        pthread_detach(thread_id);
    }
}


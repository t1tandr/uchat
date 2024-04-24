#include "server.h"

GHashTable *clients; // correct handle in multiple threads

int main(int argc, char *argv[]) {
    int sock_fd, port;

    if (argc < 2) {
        mx_printerr("usage: uchat-server <port-number>");
        exit(EXIT_FAILURE);
    }

    // start_daemon_process();

    clients = g_hash_table_new(g_str_hash, g_str_equal);

    port = atoi(argv[1]);

    init_database();
    sock_fd = start_server_socket(port);

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
    if (!SSL_CTX_use_certificate_file(ctx, "server.crt", SSL_FILETYPE_PEM)) {
        mx_printstr("hello9");
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (!SSL_CTX_use_PrivateKey_file(ctx, "server.key", SSL_FILETYPE_PEM)) {
        mx_printstr("hello10");
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        mx_printstr("hello12");
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }

    accept_clients(sock_fd, ctx);

    SSL_CTX_free(ctx);

    close(sock_fd);
}


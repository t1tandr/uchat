#include "uchat.h"

void handle_error(int type, const char* message) {
    switch (type) {
        case USAGE_ERROR:
            fprintf(stderr, "usage: usage: uchat <server-ip> <server-port>");
            break;

        case REQUEST_ERROR:
            fprintf(stderr, "uchat: failed to send \'%s\' request to server", message);
            break;

        case RESPONSE_ERROR:
            fprintf(stderr, "uchat: failed to get \'%s\' response from server", message);
            break;

        case SESSION_ERROR:
            fprintf(stderr, "uchat: failed to get session: %s", message);
            break;

        case PTHREAD_ERROR:
            fprintf(stderr, "uchat: failed create thread: %s", message);
            break;

        case BUILDER_ERROR:
            fprintf(stderr, "uchat: failed to create GtkBuilder %s: ", message);
            break;

        default:
            fprintf(stderr, "uchat: unknown error type");
            break;
    }
    
    exit(EXIT_FAILURE);
}


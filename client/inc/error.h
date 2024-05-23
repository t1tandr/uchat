#ifndef ERROR_H
#define ERROR_H

enum {
    USAGE_ERROR,
    REQUEST_ERROR,
    RESPONSE_ERROR,
    SESSION_ERROR,
    PTHREAD_ERROR, 
    BUILDER_ERROR
};

void handle_error(int type, const char* message);

#endif


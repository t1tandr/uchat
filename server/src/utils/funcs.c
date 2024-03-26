#include "server.h"

bool is_file_exists(char *filename) {
    return access(filename, F_OK) == 0;
}


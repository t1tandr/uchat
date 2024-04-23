#include "uchat.h"

void create_session_file(const char* filename, cJSON* obj) {
    FILE* file = fopen(filename, "w+");
    const char* buffer = cJSON_Print(obj);

    if (file == NULL) {
        handle_error(SESSION_ERROR, strerror(errno));
    }

    int n = fprintf(file, "%s", buffer);

    printf("%d\n", n);
}


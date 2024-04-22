#include "uchat.h"

void create_session_file(const char* filename, cJSON* obj) {
    FILE* file = fopen(filename, "w+");
    const char* buffer = cJSON_Print(obj);

    mx_printstr(buffer);
    if (file == NULL) {
        handle_error(mx_strjoin("uchat: error writing to session file: ", strerror(errno)));
    }

    fputs(buffer, file);
}


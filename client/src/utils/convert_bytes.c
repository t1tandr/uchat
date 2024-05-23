#include "uchat.h"

unsigned char* file_to_bytes(const char *file_name, long *file_size) {
    FILE *file = fopen(file_name, "rb");
    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(*file_size);
    if (buffer == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(file);
        return 0;
    }

    fread(buffer, 1, *file_size, file);
    fclose(file);

    return buffer;
}

void bytes_to_file(const unsigned char *bytes, unsigned long size, const char *output_file_name) {
    FILE *file = fopen(output_file_name, "wb");
    if (file == NULL) {
        printf("Error: Unable to create file.\n");
        return;
    }

    fwrite(bytes, 1, size, file);
    fclose(file);
    printf("Bytes successfully written to file: %s\n", output_file_name);
}
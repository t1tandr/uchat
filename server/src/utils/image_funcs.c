#include "server.h"

char *create_image(char *base64) {
    uuid_t file_id;
    char file_id_str[37];
    char file_path[250];

    uuid_generate(file_id);
    uuid_unparse_lower(file_id, file_id_str);

    create_dir_if_not_exists(IMAGES_PATH);

    snprintf(file_path, sizeof(file_path), "%s/%s.png", IMAGES_PATH, file_id_str);

    FILE *image_file = fopen(file_path, "wb");

    unsigned long base64_size;
    unsigned char *decoded_base64 = g_base64_decode(base64, &base64_size);

    fwrite(decoded_base64, 1, base64_size, image_file);

    fclose(image_file);
    g_free(decoded_base64);

    return mx_strdup(file_id_str);
}

unsigned char *get_image(char *image_id, long *size) {
    char file_path[250];

    snprintf(file_path, sizeof(file_path), "%s/%s.png", IMAGES_PATH, image_id);

    FILE* file = fopen(file_path, "rb");

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    rewind(file);

    unsigned char *buffer = (unsigned char *) malloc(*size);

    fread(buffer, 1, *size, file);

    fclose(file);
    return buffer;
}

void delete_image(char *image_id) {
    char file_path[250];

    snprintf(file_path, sizeof(file_path), "%s/%s.png", IMAGES_PATH, image_id);
    remove(file_path);
}


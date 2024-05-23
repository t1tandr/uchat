#include "server.h"

void print_unsigned_long(unsigned long num) {
    // Buffer to hold the string representation of the number
    char buffer[20]; // Assuming the number won't be larger than 20 digits

    // Convert the unsigned long to a string
    int length = sprintf(buffer, "%lu", num);

    // Write the string to the standard output
    if (write(STDOUT_FILENO, buffer, length) == -1) {
        perror("Error writing to stdout");
        exit(EXIT_FAILURE);
    }
}

void send_response(int sock_fd, cJSON *data, int status) {
    cJSON *res = cJSON_CreateObject();
    cJSON_AddNumberToObject(res, "status", status);
    cJSON_AddStringToObject(res, "type", "regular");

    if (data != NULL) {
        cJSON_AddItemToObject(res, "data", data);
    }

    char *res_str = cJSON_Print(res);
    unsigned long length = strlen(res_str);
    long nbytes = 0;

    nbytes = send(sock_fd, &length, sizeof(length), MSG_NOSIGNAL);
    
    if(nbytes > -1) {
        send(sock_fd, res_str, length, MSG_NOSIGNAL);
    }

    cJSON_Delete(res);
    cJSON_free(res_str);
}


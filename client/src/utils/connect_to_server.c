#include "uchat.h"

int connect_to_server(const char* ip, const char* port) {
    int sockfd = 0;
    struct addrinfo hints;
    struct addrinfo* servinfo = NULL;
    struct sockaddr_in* server_addr = NULL;
    char ipstr[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(ip, port, &hints, &servinfo) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    server_addr = (struct sockaddr_in *)servinfo->ai_addr;
    
    inet_ntop(servinfo->ai_family, &(server_addr->sin_addr), ipstr, sizeof(ipstr));
    printf("[CLIENT]: connected to: %s:%d\n", ipstr, ntohs(server_addr->sin_port));

    return sockfd;
}


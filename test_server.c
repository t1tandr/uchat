#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>

#include <string.h>

#include <stdio.h>

int main(void) {
    int sockfd = 0;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int yes = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    } 

    server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(1025);
	server_addr.sin_family = AF_INET;


    if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if(listen(sockfd, 5) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    while(1) {
        int client_sock;
        struct sockaddr_in client_addr;
        socklen_t size = sizeof(client_addr);
        char msg[1024];
         
        client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &size);

        if(client_sock != -1) {
            char ipstr[INET_ADDRSTRLEN];
            inet_ntop(client_addr.sin_family, &(client_addr.sin_addr), ipstr, sizeof(ipstr));
            printf("[SERVER] - connected client: %s\n", ipstr);
            
            int nbytes = recv(client_sock, msg, sizeof(msg) - sizeof(char), 0);

            if(nbytes != -1) {
                msg[nbytes] = '\0';
                printf("%s\n", msg);
                send(client_sock, "{ \"status\": 200 }", strlen("{ \"status\": 200 }"), 0);
            }
        }
    }
}
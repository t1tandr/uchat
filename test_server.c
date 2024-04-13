#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define SUCCESS_RESPONSE "{\"status\": 400}"

void* handle_request(void *arg) {
    int sockfd = *(int*)arg;

    while(1) {
        int length = 0;
        int n = 0;

        n = recv(sockfd, &length, sizeof(length), 0);

        if(n > 0) {
            printf("size = %d\n", length);

            int received_bytes = 0;
            char *res_str = malloc(length + 1);

            while(received_bytes < length) {
                n = recv(sockfd, res_str + received_bytes, length - received_bytes, 0);

                if(n < 0) {
                    continue;
                }
                received_bytes += n;
            }
            res_str[received_bytes] = '\0';
            printf("%s\n", res_str);
            
            int res_length = strlen(SUCCESS_RESPONSE);
            n = send(sockfd, &res_length, sizeof(res_length), 0);

            if(n > 0) {
                n = send(sockfd, SUCCESS_RESPONSE, strlen(SUCCESS_RESPONSE), 0);
            }

            free(res_str);
        }
    }
    close(sockfd);

    return NULL;
}

void accept_clients(int sockfd) {
    struct sockaddr_in client_addr;
    socklen_t size = sizeof(client_addr);
    char ipstr[INET_ADDRSTRLEN];
    int client_sock;
         
    while (1) {
        client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &size);

        if(client_sock < 0) {
            fprintf(stderr, "accept failed");
            exit(EXIT_FAILURE);
        }
        inet_ntop(client_addr.sin_family, &(client_addr.sin_addr), ipstr, sizeof(ipstr));
        printf("[SERVER] - connected client: %s\n", ipstr);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_request, (void *) &client_sock);
        pthread_detach(thread_id);
    }
}

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

    accept_clients(sockfd);

    close(sockfd);
}
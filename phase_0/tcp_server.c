#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFF_SIZE 10000
#define MAX_ACCEPT_BACKLOG 5

void strrev(char *str){
    for(int start = 0, end = strlen(str)-2; start<end; start++,end--){
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
    }
}

int main(){

    // Creating listening socket
    int listen_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Setting sock opt reuse addr
    int enable = 1;
    setsockopt(listen_sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    // Creating an object of struct socketaddr_in
    struct sockaddr_in server_addr;

    // Setting u server addr
    server_addr.sin_family =AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Binding listening sock to port
    bind(listen_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Starting to listen
    listen(listen_sock_fd, MAX_ACCEPT_BACKLOG);
    printf("[INFO] Server listening on port %d\n", PORT);

    /* Accepting & processing client connetions */

    // Creating an object of struct socketaddr_in
    struct sockaddr_in client_addr;
    int client_addr_len;

    // Accepting client connection
    int conn_sock_fd = accept(listen_sock_fd, (struct sockaddr *)& client_addr, &client_addr_len);
    printf("[INFO] Client connected to server\n");

    while(1){
        // Create buffer to store client message
        char buff[BUFF_SIZE];
        memset(buff,0,BUFF_SIZE);

        // Read message from client to buffer
        int read_n = recv(conn_sock_fd, buff, sizeof(buff), 0);

        // Client closed connection or error occurred
        if(read_n <= 0){
            printf("[INFO] Client disconnected. Closing server\n");
            close(conn_sock_fd);
            exit(1);
        }

        printf("[CLIENT MESSAGE] %s", buff);

        strrev(buff);

        send(conn_sock_fd, buff, read_n, 0);
    }

    return 0;
}

/*
Notes:
1. socket(domain, type, protocol)
    AF_INET: use of IPv4 address family
    SOCK_STREAM: it provides a reliable, connection-oriented, and sequenced flow
    0: used for TCP, (when AF_INET and SOCK_STREAM are chosen)
*/
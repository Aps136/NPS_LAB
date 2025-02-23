#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Server port to connect to
int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];  // Buffer to store server response
    char str[550];
    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  // Convert to network byte order
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP (localhost)
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    while(1){
        printf("enter a string : ");
        scanf("%s",str );
        if(strcmp(str, "exit")==0){
            break;
        }
        send(sockfd, str, strlen(str)+1, 0);
        printf("sent to the server\n");
        recv(sockfd, str,strlen(str)+1, 0);
        printf("%s\n", str);
    }
    // Close the socket
    close(sockfd);

    return 0;
}

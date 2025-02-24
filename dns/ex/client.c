#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Server port to connect to
#define BUFF_SIZE 1024
int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024]; 
    char input[1024];

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
    printf("enter a choice: \n1.Reg No\n2.Name\n3.Subject\n4.exit\n");
    fgets(input, BUFF_SIZE, stdin);
    input[strcspn(input, "\n")]=0;
    send(sockfd, input, strlen(input)+1, 0);
    memset(buffer, 0, BUFF_SIZE);
    recv(sockfd, buffer, BUFF_SIZE, 0);
    printf("Server says: %s\n", buffer);

    // Close the socket
    close(sockfd);

    return 0;
}

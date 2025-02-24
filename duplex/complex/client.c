#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 1234
#define BUFF_SIZE 550

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFF_SIZE];
    char input[BUFF_SIZE];

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Get user input
    printf("Enter an alphanumeric string: ");
    fgets(input, BUFF_SIZE, stdin);
    input[strcspn(input, "\n")] = 0;  // Remove newline character

    // Send input to server
    send(sockfd, input, strlen(input) + 1, 0);

    // Receive response from child process
    memset(buffer, 0, BUFF_SIZE);
    recv(sockfd, buffer, BUFF_SIZE, 0);
    printf("Child Process Response: %s\n", buffer);

    // Receive response from parent process
    memset(buffer, 0, BUFF_SIZE);
    recv(sockfd, buffer, BUFF_SIZE, 0);
    printf("Parent Process Response: %s\n", buffer);

    close(sockfd);
    return 0;
}

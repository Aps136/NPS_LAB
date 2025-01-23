#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345  // Server port
#define MAX_LEN 1024 // Maximum length of the string

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_LEN];
    socklen_t addr_len;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Localhost

    addr_len = sizeof(server_addr);

    printf("Client is ready. Enter 'Halt' to terminate.\n");

    while (1) {
        printf("Enter a string: ");
        fgets(buffer, MAX_LEN, stdin);

        // Remove the newline character if it's read by fgets
        buffer[strcspn(buffer, "\n")] = 0;

        // Send the string to the server
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&server_addr, addr_len);

        // If the string is "Halt", break the loop and terminate
        if (strcmp(buffer, "Halt") == 0) {
            break;
        }

        // Receive the result from the server
        int n = recvfrom(sockfd, buffer, MAX_LEN, 0, (struct sockaddr *)&server_addr, &addr_len);
        buffer[n] = '\0';

        // Display the result
        printf("Received from server:\n%s\n", buffer);
    }

    // Close the socket
    close(sockfd);
    return 0;
}

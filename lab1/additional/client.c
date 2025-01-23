#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1234

void sendTCPMessage();
void sendUDPMessage();

int main() {
    int choice;

    printf("Choose the connection type:\n");
    printf("1. TCP (Connection-Oriented)\n");
    printf("2. UDP (Connectionless)\n");
    scanf("%d", &choice);

    if (choice == 1) {
        sendTCPMessage();
    } else if (choice == 2) {
        sendUDPMessage();
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}

void sendTCPMessage() {
    int sockfd;
    struct sockaddr_in server_addr;

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("TCP socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP (localhost)

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("TCP connection failed");
        close(sockfd);
        exit(1);
    }

    printf("Connected to TCP server.\n");
    close(sockfd);
}

void sendUDPMessage() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[] = "Hello, UDP Server!";

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("UDP socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP (localhost)

    // Send a message to the server
    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Message sent to UDP server.\n");

    close(sockfd);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 1234

void handleTCPConnection();
void handleUDPConnection();

int main() {
    int choice;

    printf("Choose the connection type:\n");
    printf("1. TCP (Connection-Oriented)\n");
    printf("2. UDP (Connectionless)\n");
    scanf("%d", &choice);

    if (choice == 1) {
        handleTCPConnection();
    } else if (choice == 2) {
        handleUDPConnection();
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}

void handleTCPConnection() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char client_ip[INET_ADDRSTRLEN];
    int client_port;

    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("TCP socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("TCP binding failed");
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("TCP listen failed");
        close(server_fd);
        exit(1);
    }

    printf("TCP server listening on port %d...\n", PORT);

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (client_fd < 0) {
        perror("TCP accept failed");
        close(server_fd);
        exit(1);
    }

    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    client_port = ntohs(client_addr.sin_port);

    printf("Connected to client:\n");
    printf("IP Address: %s\n", client_ip);
    printf("Port: %d\n", client_port);

    close(client_fd);
    close(server_fd);
}

void handleUDPConnection() {
    int udp_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char client_ip[INET_ADDRSTRLEN];
    int client_port;
    char buffer[1024];

    // Create a UDP socket
    udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_fd < 0) {
        perror("UDP socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(udp_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("UDP binding failed");
        close(udp_fd);
        exit(1);
    }

    printf("UDP server listening on port %d...\n", PORT);

    addr_size = sizeof(client_addr);
    recvfrom(udp_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_size);

    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    client_port = ntohs(client_addr.sin_port);

    printf("Received data from client:\n");
    printf("IP Address: %s\n", client_ip);
    printf("Port: %d\n", client_port);

    close(udp_fd);
}

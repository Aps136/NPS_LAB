#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to find IP address for a given domain
void find_ip(char *domain, char *ip) {
    FILE *file = fopen("database.txt", "r");
    if (!file) {
        perror("Database file missing");
        strcpy(ip, "ERROR: Database file missing");
        return;
    }

    char file_domain[BUFFER_SIZE], file_ip[BUFFER_SIZE];
    while (fscanf(file, "%s %s", file_domain, file_ip) != EOF) {
        if (strcmp(domain, file_domain) == 0) {
            strcpy(ip, file_ip);
            fclose(file);
            return;
        }
    }

    strcpy(ip, "ERROR: Domain not found");
    fclose(file);
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE], ip[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind and listen
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }
    listen(server_fd, 5);
    printf("DNS Server is running on port %d...\n", PORT);

    // Accept and process client requests
    while (1) {
        client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("Connection failed");
            continue;
        }

        // Receive domain query from client
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Client requested domain: %s\n", buffer);

        // Find IP and send response
        find_ip(buffer, ip);
        send(client_sock, ip, sizeof(ip), 0);
        printf("Sent response: %s\n", ip);

        close(client_sock);
    }

    close(server_fd);
    return 0;
}

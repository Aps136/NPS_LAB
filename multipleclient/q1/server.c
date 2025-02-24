#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Define the port on which the server will listen
#define MAX_CLIENTS 2  // Max number of allowed clients
#define BUFF_SIZE 1024

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFF_SIZE];

    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  // Convert to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; // Allow any incoming address
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) == 0) {
        printf("Server is listening on port %d...\n", PORT);
    } else {
        perror("Error in listening");
        exit(1);
    }

    // Open file and initialize with "Manipal "
    FILE *file = fopen("data.txt", "w");
    if (file) {
        fprintf(file, "Manipal ");
        fclose(file);
    }

    int client_count = 0;

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("Connection failed");
            continue;
        }

        client_count++;

        if (client_count > MAX_CLIENTS) {
            char msg[] = "terminate session";
            send(client_sock, msg, sizeof(msg), 0);
            close(client_sock);
            break; // Stop accepting new connections
        }

        // Receive message from client
        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Received: %s\n", buffer);

        // Append to file
        file = fopen("data.txt", "a");
        if (file) {
            fprintf(file, "%s ", buffer);
            fclose(file);
        }

        // Read and send updated content
        file = fopen("data.txt", "r");
        if (file) {
            fread(buffer, 1, sizeof(buffer), file);
            fclose(file);
        }

        send(client_sock, buffer, sizeof(buffer), 0);
        printf("Updated text sent: %s\n", buffer);

        close(client_sock);
    }

    close(server_fd);
    return 0;
}

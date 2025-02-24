#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 1234
#define BUFF_SIZE 550

void sort(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (str[j] > str[j + 1]) {
                char temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }
}

void sort_desc(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (str[j] < str[j + 1]) {
                char temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }
}

void handle_client(int client_sock) {
    char buffer[BUFF_SIZE];

    // Read string from client
    memset(buffer, 0, BUFF_SIZE);
    read(client_sock, buffer, BUFF_SIZE);

    // Fork process
    pid_t pid = fork();

    if (pid == 0) {  // Child process (Sort numbers in ascending order)
        printf("Child Process (PID: %d) Sorting Numbers in Ascending Order...\n", getpid());
        
        char sorted_numbers[BUFF_SIZE];
        int idx = 0;

        // Extract numbers only
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] >= '0' && buffer[i] <= '9') {
                sorted_numbers[idx++] = buffer[i];
            }
        }
        sorted_numbers[idx] = '\0';

        sort(sorted_numbers);

        // Send result back to client
        char response[BUFF_SIZE];
        snprintf(response, sizeof(response), "PID: %d, Sorted Numbers: %s", getpid(), sorted_numbers);
        write(client_sock, response, strlen(response) + 1);

    } else {  // Parent process (Sort characters in descending order)
        printf("Parent Process (PID: %d) Sorting Characters in Descending Order...\n", getpid());

        char sorted_chars[BUFF_SIZE];
        int idx = 0;

        // Extract alphabets only
        for (int i = 0; buffer[i] != '\0'; i++) {
            if ((buffer[i] >= 'A' && buffer[i] <= 'Z') || (buffer[i] >= 'a' && buffer[i] <= 'z')) {
                sorted_chars[idx++] = buffer[i];
            }
        }
        sorted_chars[idx] = '\0';

        sort_desc(sorted_chars);

        // Send result back to client
        char response[BUFF_SIZE];
        snprintf(response, sizeof(response), "PID: %d, Sorted Characters: %s", getpid(), sorted_chars);
        write(client_sock, response, strlen(response) + 1);
    }

    close(client_sock);
}

int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    // Listen for connections
    if (listen(server_fd, 5) == 0) {
        printf("Server is listening on port %d...\n", PORT);
    } else {
        perror("Error in listening");
        exit(1);
    }

    // Accept and handle client connections
    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("Connection failed");
            continue;
        }
        handle_client(client_sock);
    }

    close(server_fd);
    return 0;
}

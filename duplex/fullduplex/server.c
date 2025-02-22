#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 1234  // Define the port on which the server will listen
#define BUFFER_SIZE 1024

void chat(int sockfd){
    char buffer[BUFFER_SIZE];
    pid_t pid =fork();
    if(pid==0){
        printf("Receiver Process PID: %d, PPID: %d\n", getpid(), getppid());
        while(1){
            memset(buffer, 0, BUFFER_SIZE);
            read(sockfd, buffer, BUFFER_SIZE);
            if(strcmp(buffer, "exit")==0){
                printf("chat ended by peer\n");
                break;
            }
            printf("peer says: %s\n", buffer);
        }
    }else{
        printf("sender process pid: %d, PPID : %d\n", getpid(), getppid());
        while(1){
            memset(buffer ,0, BUFFER_SIZE);
            printf("Sender:  ");
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")]=0;
            write(sockfd, buffer, strlen(buffer)+1);
            if(strcmp(buffer, "exit")==0){
                printf("chat ended\n");
                break;
            }
        }
    }close(sockfd);
}



int main() {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
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
    if (listen(server_fd, 5) == 0) {
        printf("Server is listening on port %d...\n", PORT);
    } else {
        perror("Error in listening");
        exit(1);
    }

    addr_size = sizeof(client_addr);
    new_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (new_sock < 0) {
        perror("Connection failed");
        exit(1);
    }
    printf("Message sent to client\n");
    chat(new_sock);
    // Close the socket
   
    close(server_fd);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFF_SIZE 1024
#define PORT 1234  // Define the port on which the server will listen
void handle_client(int client_sock){
    char buffer[BUFF_SIZE];

    memset(buffer, 0, BUFF_SIZE);
    read(client_sock, buffer, BUFF_SIZE);
    int option = buffer[0]-'0';
    char response[BUFF_SIZE];
    pid_t pid= fork();
    if(pid==0){
        if(option==1){
            snprintf(response, sizeof(response),"PID: %d, Name: JOHN DOE, Address: MANIPAL", getpid());

        }
        else if(option==2){
            snprintf(response, sizeof(response),"PID: %d, Dept: CSE, Sem 5, Section: A, Courses: DS", getpid());

        }
        else if(option==3){
            snprintf(response, sizeof(response),"PID: %d, Subject: DBMS, Marks: 100", getpid());

        }
        else if(option==4){
            printf("bye\n");
            exit(0);
        }
        else{
            snprintf(response, sizeof(response),"Invalid option!");
        }
        write(client_sock, response, strlen(response)+1);
        exit(0);
    }
    close(client_sock);
}
int main() {
    int server_fd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char *message = "Hello, Client!"; // Message to send to the client

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
    while(1){
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

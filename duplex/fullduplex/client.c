#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 1234  // Server port to connect to
#define BUFFER_SIZE 1024
void chat(int sockfd){
    char buffer[BUFFER_SIZE];
    pid_t pid = fork();
    if(pid==0){
        printf("receiver process pid: %d, PPID %d\n", getpid(), getppid());
        while(1){
            memset(buffer, 0, BUFFER_SIZE);
            read(sockfd, buffer, BUFFER_SIZE);
            if(strcmp(buffer, "exit")==0){
                printf("exiting\n");
                break;
            }
            printf("peer: %s\n", buffer);
        }
    }else{
        printf("sender process pid %d ppid %d \n", getpid(), getppid());
        while(1){
            memset(buffer, 0, BUFFER_SIZE);
            printf("you: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")]=0;
            write(sockfd, buffer, strlen(buffer)+1);
            if(strcmp(buffer, "exit")==0){
                printf("chat ended\n");
                break;
            }
        }close(sockfd);
    }
}
int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];  // Buffer to store server response

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

    printf("connected to server\n");
    chat(sockfd);


    return 0;
}

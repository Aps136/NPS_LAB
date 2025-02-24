#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345  // Port to send data to
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char str[BUF_SIZE];
   
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Fill server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address

    // Send message to the server
    while(1){
        printf("Enter a string : ");
        scanf("%s", str);
        int x=sendto(sockfd, (const char *)str, strlen(str), MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        if(x==-1){
            printf("failure in sending the string\n");
            close(sockfd);
            exit(EXIT_FAILURE);

        }
        printf("string sent to the server\n");
        if(strcmp(str, "exit")==0){
            printf("exiting...");
            break;
        }
    }
    
   
    close(sockfd);
    return 0;
}

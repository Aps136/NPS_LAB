#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345  // Port to listen on
#define BUF_SIZE 1024
void swap(char *x, char *y){
    char temp= *x;
    *x=*y;
    *y=temp;
}
void permute(char *a, int l, int r){
    int i;
    if(l==r){
        printf("\n%s", a);

    }
    else{
        for(i=l;i<=r;i++){
            swap((a+l),(a+i));
            permute(a, l+1, r);
            swap((a+l),(a+i));
        }
    }
}
int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Fill server information
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for data on port %d...\n", PORT);

    while (1) {
        // Receive data from client
        int recb=recvfrom(sockfd, buffer,sizeof(buffer)-1, 0, (struct sockaddr*)&client_addr, &addr_len );
        if(recb==-1){
            printf("message receiving failed\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        buffer[recb] = '\0';  // Null-terminate the received data
        printf("Received message from client: %s\n", buffer);
        if(strcmp(buffer, "stop")==0){
            break;
        }
        permute(buffer, 0,strlen(buffer)-1);
        // Send a response back to the client
        //sendto(sockfd, (const char *)"Message received", strlen("Message received"), MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len);
        //printf("Sent acknowledgment to client.\n");
    }

    close(sockfd);
    return 0;
}

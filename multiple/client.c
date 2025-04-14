#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 100
#define PORT 3388

int main() {
    int sockfd, retval;
    struct sockaddr_in serveraddr;
    char buff[MAX];
    int recedbytes, sentbytes;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation error\n");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Connection error\n");
        close(sockfd);
        exit(1);
    }

    int route, seats;
    printf("Enter route number (1 or 2): ");
    scanf("%d", &route);
    printf("Enter number of seats to book: ");
    scanf("%d", &seats);

    sprintf(buff, "%d %d", route, seats);
    sentbytes = send(sockfd, buff, sizeof(buff), 0);

    if (sentbytes == -1) {
        printf("Send error\n");
        close(sockfd);
        exit(1);
    }

    recedbytes = recv(sockfd, buff, sizeof(buff), 0);
    if (recedbytes == -1) {
        printf("Receive error\n");
    } else {
        printf("Server Response: %s\n", buff);
    }

    close(sockfd);
    return 0;
}

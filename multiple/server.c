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
    int sockfd, newsockfd;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    int recedbytes, sentbytes;
    char buff[MAX];

    // Booking database
    int seats_available[2] = {10, 10};  // Route 1 and Route 2
    int seats_booked[2] = {0, 0};

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation error\n");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        printf("Binding error\n");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) == -1) {
        printf("Listen error\n");
        close(sockfd);
        exit(1);
    }

    printf("Travel Ticket Reservation Server running...\n");

    while (1) {
        len = sizeof(clientaddr);
        newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
        if (newsockfd == -1) {
            printf("Accept error\n");
            continue;
        }

        // Show current availability
        printf("\nCurrent Seat Status:\n");
        printf("Route 1: %d booked, %d available\n", seats_booked[0], seats_available[0]);
        printf("Route 2: %d booked, %d available\n", seats_booked[1], seats_available[1]);

        // Receive client route and seats request
        recedbytes = recv(newsockfd, buff, sizeof(buff), 0);
        if (recedbytes <= 0) {
            close(newsockfd);
            continue;
        }

        int route, seats_requested;
        sscanf(buff, "%d %d", &route, &seats_requested);

        char response[MAX];

        if (route < 1 || route > 2) {
            sprintf(response, "Invalid route selected.");
        } else if (seats_requested <= seats_available[route - 1]) {
            seats_available[route - 1] -= seats_requested;
            seats_booked[route - 1] += seats_requested;
            sprintf(response, "Booking successful! %d seat(s) reserved on Route %d.", seats_requested, route);
        } else {
            sprintf(response, "Booking failed. Only %d seat(s) available on Route %d.", seats_available[route - 1], route);
        }

        send(newsockfd, response, sizeof(response), 0);
        close(newsockfd);
    }

    close(sockfd);
    return 0;
}

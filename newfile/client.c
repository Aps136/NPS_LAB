// client.c
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

struct book {
    int id;
    char title[100];
    char author[50];
    int year;
};

int main() {
    int choice;
    struct book b;
    int sockfd, retval;
    struct sockaddr_in serveraddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation error");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4000);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Connection error");
        close(sockfd);
        exit(1);
    }

    while (1) {
        printf("\nMENU:\n1. Insert\n2. Display\n3. Update\n4. Delete\n5. Search\n6. Exit\nEnter choice: ");
        scanf("%d", &choice);
        send(sockfd, &choice, sizeof(choice), 0);

        if (choice == 1) {
            printf("Enter ID: ");
            scanf("%d", &b.id);
            printf("Enter Title: ");
            scanf(" %[^\n]", b.title);
            printf("Enter Author: ");
            scanf(" %[^\n]", b.author);
            printf("Enter Year: ");
            scanf("%d", &b.year);
            send(sockfd, &b, sizeof(b), 0);
        } else if (choice == 3 || choice == 4 || choice == 5) {
            printf("Enter ID: ");
            scanf("%d", &b.id);
            send(sockfd, &b.id, sizeof(b.id), 0);
            if (choice == 3) {
                printf("Enter updated Title: ");
                scanf(" %[^\n]", b.title);
                printf("Enter updated Author: ");
                scanf(" %[^\n]", b.author);
                printf("Enter updated Year: ");
                scanf("%d", &b.year);
                send(sockfd, &b, sizeof(b), 0);
            }
        } else if (choice == 6) {
            printf("Exiting...\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}

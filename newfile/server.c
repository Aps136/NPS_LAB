// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

struct book {
    int id;
    char title[100];
    char author[50];
    int year;
};

int main() {
    int sockfd, newsockfd, retval;
    socklen_t actuallen;

    struct book b;
    FILE *fp, *temp;
    int id, choice;

    struct sockaddr_in serveraddr, clientaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation error");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4000);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Binding error");
        close(sockfd);
        exit(1);
    }

    retval = listen(sockfd, 1);
    if (retval == -1) {
        perror("Listen error");
        close(sockfd);
        exit(1);
    }

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1) {
        perror("Accept error");
        close(sockfd);
        exit(1);
    }

    while (1) {
        recv(newsockfd, &choice, sizeof(choice), 0);
        switch (choice) {
            case 1: // Insert
                recv(newsockfd, &b, sizeof(b), 0);
                fp = fopen("file.txt", "a");
                if (fp == NULL) {
                    perror("File open error");
                    break;
                }
                fprintf(fp, "%d|%s|%s|%d\n", b.id, b.title, b.author, b.year);
                fclose(fp);
                printf("Inserted record!\n");
                break;

            case 2: // Display
                fp = fopen("file.txt", "r");
                if (fp == NULL) {
                    perror("File open error");
                    break;
                }
                printf("Book Records:\n");
                while (fscanf(fp, "%d|%[^|]|%[^|]|%d\n", &b.id, b.title, b.author, &b.year) == 4) {
                    printf("ID: %d\nTitle: %s\nAuthor: %s\nYear: %d\n\n", b.id, b.title, b.author, b.year);
                }
                fclose(fp);
                break;

            case 3: // Update
                recv(newsockfd, &id, sizeof(id), 0);
                recv(newsockfd, &b, sizeof(b), 0);
                fp = fopen("file.txt", "r");
                temp = fopen("temp.txt", "w");
                if (fp == NULL || temp == NULL) {
                    perror("File open error");
                    break;
                }
                int found = 0;
                struct book tempb;
                while (fscanf(fp, "%d|%[^|]|%[^|]|%d\n", &tempb.id, tempb.title, tempb.author, &tempb.year) == 4) {
                    if (tempb.id == id) {
                        fprintf(temp, "%d|%s|%s|%d\n", b.id, b.title, b.author, b.year);
                        found = 1;
                    } else {
                        fprintf(temp, "%d|%s|%s|%d\n", tempb.id, tempb.title, tempb.author, tempb.year);
                    }
                }
                fclose(fp);
                fclose(temp);
                remove("file.txt");
                rename("temp.txt", "file.txt");
                if (!found) printf("Record not found!\n");
                else printf("Record updated!\n");
                break;

            case 4: // Delete
                recv(newsockfd, &id, sizeof(id), 0);
                fp = fopen("file.txt", "r");
                temp = fopen("temp.txt", "w");
                if (fp == NULL || temp == NULL) {
                    perror("File open error");
                    break;
                }
                found = 0;
                while (fscanf(fp, "%d|%[^|]|%[^|]|%d\n", &b.id, b.title, b.author, &b.year) == 4) {
                    if (b.id == id) {
                        found = 1;
                        continue;
                    }
                    fprintf(temp, "%d|%s|%s|%d\n", b.id, b.title, b.author, b.year);
                }
                fclose(fp);
                fclose(temp);
                remove("file.txt");
                rename("temp.txt", "file.txt");
                if (!found) printf("Record not found!\n");
                else printf("Record deleted!\n");
                break;

            case 5: // Search
                recv(newsockfd, &id, sizeof(id), 0);
                fp = fopen("file.txt", "r");
                if (fp == NULL) {
                    perror("File open error");
                    break;
                }
                found = 0;
                while (fscanf(fp, "%d|%[^|]|%[^|]|%d\n", &b.id, b.title, b.author, &b.year) == 4) {
                    if (b.id == id) {
                        printf("Found Record:\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\n", b.id, b.title, b.author, b.year);
                        found = 1;
                        break;
                    }
                }
                fclose(fp);
                if (!found) printf("Record not found!\n");
                break;

            case 6: // Exit
                printf("Server shutting down...\n");
                close(newsockfd);
                close(sockfd);
                exit(0);
        }
    }

    return 0;
}

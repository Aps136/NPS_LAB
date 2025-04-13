// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define MAXSIZE 100

void sort(int *arr, int n, int order) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if ((order == 1 && arr[j] > arr[j+1]) || (order == 2 && arr[j] < arr[j+1])) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main() {
    int sockfd, newsockfd, retval;
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;

    int arr[MAXSIZE], n, choice, target, found;
    int even[MAXSIZE], odd[MAXSIZE], evenCount = 0, oddCount = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation error\n");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    retval = bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Binding error\n");
        close(sockfd);
        return 1;
    }

    listen(sockfd, 1);
    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&clientaddr, &actuallen);
    if (newsockfd == -1) {
        printf("Accept error\n");
        close(sockfd);
        return 1;
    }

    while (1) {
        recv(newsockfd, &choice, sizeof(choice), 0);
        if (choice == 4) {
            printf("Client exited.\n");
            break;
        }

        recv(newsockfd, &n, sizeof(n), 0);
        recv(newsockfd, arr, sizeof(int)*n, 0);

        if (choice == 1) { // Search
            recv(newsockfd, &target, sizeof(target), 0);
            found = 0;
            for (int i = 0; i < n; i++) {
                if (arr[i] == target) {
                    found = 1;
                    break;
                }
            }
            send(newsockfd, &found, sizeof(found), 0);
        }
        else if (choice == 2) { // Sort
            int order;
            recv(newsockfd, &order, sizeof(order), 0); // 1 for ascending, 2 for descending
            sort(arr, n, order);
            send(newsockfd, arr, sizeof(int)*n, 0);
        }
        else if (choice == 3) { // Split
            evenCount = oddCount = 0;
            for (int i = 0; i < n; i++) {
                if (arr[i] % 2 == 0)
                    even[evenCount++] = arr[i];
                else
                    odd[oddCount++] = arr[i];
            }
            send(newsockfd, &evenCount, sizeof(evenCount), 0);
            send(newsockfd, even, sizeof(int)*evenCount, 0);
            send(newsockfd, &oddCount, sizeof(oddCount), 0);
            send(newsockfd, odd, sizeof(int)*oddCount, 0);
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

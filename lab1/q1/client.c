// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#define MAXSIZE 100

int main() {
    int sockfd, retval;
    struct sockaddr_in serveraddr;
    int arr[MAXSIZE], n, choice, target, result;
    int even[MAXSIZE], odd[MAXSIZE], evenCount, oddCount;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation error\n");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Connection error\n");
        close(sockfd);
        return 1;
    }

    while (1) {
        printf("\n1. Search for number\n2. Sort\n3. Split into Odd and Even\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        send(sockfd, &choice, sizeof(choice), 0);

        if (choice == 4) {
            printf("Exiting...\n");
            break;
        }

        printf("Enter number of elements: ");
        scanf("%d", &n);
        printf("Enter %d integers: ", n);
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }

        send(sockfd, &n, sizeof(n), 0);
        send(sockfd, arr, sizeof(int)*n, 0);

        if (choice == 1) { // Search
            printf("Enter number to search: ");
            scanf("%d", &target);
            send(sockfd, &target, sizeof(target), 0);
            recv(sockfd, &result, sizeof(result), 0);
            if (result)
                printf("Number found!\n");
            else
                printf("Number not found!\n");
        }
        else if (choice == 2) { // Sort
            int order;
            printf("1. Ascending\n2. Descending\nEnter sort order: ");
            scanf("%d", &order);
            send(sockfd, &order, sizeof(order), 0);
            recv(sockfd, arr, sizeof(int)*n, 0);
            printf("Sorted array: ");
            for (int i = 0; i < n; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
        }
        else if (choice == 3) { // Split
            recv(sockfd, &evenCount, sizeof(evenCount), 0);
            recv(sockfd, even, sizeof(int)*evenCount, 0);
            recv(sockfd, &oddCount, sizeof(oddCount), 0);
            recv(sockfd, odd, sizeof(int)*oddCount, 0);

            printf("Even numbers: ");
            for (int i = 0; i < evenCount; i++)
                printf("%d ", even[i]);
            printf("\nOdd numbers: ");
            for (int i = 0; i < oddCount; i++)
                printf("%d ", odd[i]);
            printf("\n");
        }
    }

    close(sockfd);
    return 0;
}

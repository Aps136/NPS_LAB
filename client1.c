#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234  // Server port to connect to

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    int n, choice, num, *arr;

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP (localhost)
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    // Communicate with the server
    while (1) {
        printf("Enter the number of integers: ");
        scanf("%d", &n);

        arr = (int *)malloc(n * sizeof(int));
        printf("Enter the integers:\n");
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }

        printf("Choose an operation:\n");
        printf("1. Search for a number\n2. Sort in ascending order\n3. Sort in descending order\n4. Split into odd and even\n5. Exit\n");
        scanf("%d", &choice);

        if (choice == 5) {
            send(sockfd, &n, sizeof(n), 0);  // Send number of elements
            send(sockfd, arr, n * sizeof(int), 0);  // Send array
            send(sockfd, &choice, sizeof(choice), 0);  // Send choice to exit
            printf("Exiting...\n");
            break;
        }

        send(sockfd, &n, sizeof(n), 0);  // Send number of elements
        send(sockfd, arr, n * sizeof(int), 0);  // Send array
        send(sockfd, &choice, sizeof(choice), 0);  // Send choice

        if (choice == 1) {
            printf("Enter the number to search for: ");
            scanf("%d", &num);
            send(sockfd, &num, sizeof(num), 0);  // Send the number to search

            int found;
            recv(sockfd, &found, sizeof(found), 0);  // Receive the search result
            if (found) {
                printf("Number %d found in the set.\n", num);
            } else {
                printf("Number %d not found in the set.\n", num);
            }
        } else if (choice == 2 || choice == 3) {
            recv(sockfd, arr, n * sizeof(int), 0);  // Receive the sorted array
            printf("Resulting array: ");
            for (int i = 0; i < n; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
        } else if (choice == 4) {
            int oddCount, evenCount;
            int *odd, *even;

            recv(sockfd, &oddCount, sizeof(oddCount), 0);  // Receive odd count
            odd = (int *)malloc(oddCount * sizeof(int));
            recv(sockfd, odd, oddCount * sizeof(int), 0);  // Receive odd numbers

            recv(sockfd, &evenCount, sizeof(evenCount), 0);  // Receive even count
            even = (int *)malloc(evenCount * sizeof(int));
            recv(sockfd, even, evenCount * sizeof(int), 0);  // Receive even numbers

            printf("Odd numbers: ");
            for (int i = 0; i < oddCount; i++) {
                printf("%d ", odd[i]);
            }
            printf("\n");

            printf("Even numbers: ");
            for (int i = 0; i < evenCount; i++) {
                printf("%d ", even[i]);
            }
            printf("\n");

            free(odd);
            free(even);
        } else {
            printf("Invalid choice.\n");
        }

        free(arr);
    }

    // Close the socket
    close(sockfd);

    return 0;
}

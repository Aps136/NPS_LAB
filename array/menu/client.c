#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234  // Server port to connect to
#define MAX_SIZE 100

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    int n, choice, target;
    int ar[MAX_SIZE];

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

    printf("Enter number of elements: ");
    scanf("%d", &n);
    
    printf("Enter values: \n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &ar[i]);
    }

    send(sockfd, &n, sizeof(n), 0);
    send(sockfd, ar, n * sizeof(int), 0);
    printf("Array sent to server\n");
    
    // Choose an operation
    printf("Choose operation: 1. Sort 2. Reverse 3. Sum 4. Count Occurrences 5. Count Primes\n");
    scanf("%d", &choice);
    send(sockfd, &choice, sizeof(choice), 0);

    if (choice == 4) {  // Count occurrences requires a target value
        printf("Enter number to count occurrences: ");
        scanf("%d", &target);
        send(sockfd, &target, sizeof(target), 0);
    }

    int result;
    if (choice == 3 || choice == 5) { // Sum or Count Primes returns an integer result
        recv(sockfd, &result, sizeof(result), 0);
        printf("Result: %d\n", result);
    } else {
        recv(sockfd, ar, n * sizeof(int), 0);
        printf("Processed array: \n");
        for (int i = 0; i < n; i++) {
            printf("%d\t", ar[i]);
        }
        printf("\n");
    }
    
    // Close the socket
    close(sockfd);
    return 0;
}

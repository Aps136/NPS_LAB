#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Define the port on which the server will listen

void sortAscending(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void sortDescending(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] < arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int searchNumber(int *arr, int n, int num) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == num) {
            return 1;  
        }
    }
    return 0;  
}

void splitOddEven(int *arr, int n, int **odd, int *oddCount, int **even, int *evenCount) {
    *oddCount = *evenCount = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) (*evenCount)++;
        else (*oddCount)++;
    }

    *odd = malloc((*oddCount) * sizeof(int));
    *even = malloc((*evenCount) * sizeof(int));

    int o = 0, e = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) (*even)[e++] = arr[i];
        else (*odd)[o++] = arr[i];
    }
}

int main() {
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int n, choice, num;
    int *arr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    if (listen(server_fd, 5) == 0) {
        printf("Server is listening on port %d...\n", PORT);
    } else {
        perror("Error in listening");
        exit(1);
    }

    addr_size = sizeof(client_addr);
    new_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (new_sock < 0) {
        perror("Connection failed");
        exit(1);
    }

    while (1) {
        recv(new_sock, &n, sizeof(n), 0);  // Receive the number of elements
        arr = (int *)malloc(n * sizeof(int));
        recv(new_sock, arr, n * sizeof(int), 0);  // Receive the array
        recv(new_sock, &choice, sizeof(choice), 0);  // Receive the choice

        if (choice == 5) {
            printf("Client disconnected.\n");
            free(arr);
            break;
        }

        switch (choice) {
            case 1: {  // Search for a number
                recv(new_sock, &num, sizeof(num), 0);
                int found = searchNumber(arr, n, num);
                send(new_sock, &found, sizeof(found), 0);
                break;
            }
            case 2:  // Sort in ascending order
                sortAscending(arr, n);
                send(new_sock, arr, n * sizeof(int), 0);
                break;
            case 3:  // Sort in descending order
                sortDescending(arr, n);
                send(new_sock, arr, n * sizeof(int), 0);
                break;
            case 4: {  // Split into odd and even
                int *odd, *even, oddCount, evenCount;
                splitOddEven(arr, n, &odd, &oddCount, &even, &evenCount);
                send(new_sock, &oddCount, sizeof(oddCount), 0);
                send(new_sock, odd, oddCount * sizeof(int), 0);
                send(new_sock, &evenCount, sizeof(evenCount), 0);
                send(new_sock, even, evenCount * sizeof(int), 0);
                free(odd);
                free(even);
                break;
            }
            default:
                printf("Invalid choice received.\n");
                break;
        }

        free(arr);
    }

    close(new_sock);
    close(server_fd);

    return 0;
}

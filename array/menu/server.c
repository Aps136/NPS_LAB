#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Define the port on which the server will listen
#define MAX_SIZE 100
void sort(int *a, int n){
    for(int i=0;i<n;i++){
        for(int j=0;j<n-i-1;j++){
            if(a[j]>a[j+1]){
                int temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;

            }
            
        }
    }
}
int count(int *a, int n, int target){
    int c=0;
    for(int i=0;i<n;i++){
        if(a[i]==target){
            c++;
        }
    }
    return c;
}
int sum(int * a, int n){
    int c=0;
    for(int i=0;i<n;i++){
        c+=a[i];
    }
    return c;
}

int is_prime(int num) {
    if (num < 2) return 0;  // 0 and 1 are not prime
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;  // Not prime
        }
    }
    return 1;  // Prime
}

int count_primes(int *a, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (is_prime(a[i])) {
            count++;  // Count prime numbers
        }
    }
    return count;
}
void occ(int * a, int n){
    int freq[256]={0};
    for(int i=0;i<n;i++){
        freq[a[i]]++;

    }
    for(int i=0;i<256;i++){
        if(freq[i]>0){
            printf("%doccurs %d times\n", i, freq[i]);
        }
    }
}
void rev(int * a, int n){
    for(int i=0;i<n/2;i++){
        int temp=a[i];
        a[i]=a[n-i-1];
        a[n-i-1]=temp;
    }
}

int main() {
    int n;
    int ar[MAX_SIZE];
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  // Convert to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; // Allow any incoming address
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        exit(1);
    }

    // Start listening for incoming connections
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
    recv(new_sock, &n, sizeof(n),0);
    recv(new_sock, ar, n*sizeof(int),0);
    sort(ar, n);
    printf("sorted\n");
    send(new_sock, ar, n*sizeof(int),0);
   
    // Close the socket
    close(new_sock);
    close(server_fd);

    return 0;
}

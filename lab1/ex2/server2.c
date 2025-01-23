#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345  
#define MAX_LEN 1024 

// Function to check if a string is a palindrome
int isPalindrome(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;  // Not a palindrome
        }
    }
    return 1;  // Is a palindrome
}

// Function to count occurrences of vowels in a string
void countVowels(char *str, int *vowelCount) {
    for (int i = 0; str[i] != '\0'; i++) {
        char ch = tolower(str[i]);
        if (ch == 'a') vowelCount[0]++;
        else if (ch == 'e') vowelCount[1]++;
        else if (ch == 'i') vowelCount[2]++;
        else if (ch == 'o') vowelCount[3]++;
        else if (ch == 'u') vowelCount[4]++;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[MAX_LEN];
    socklen_t addr_len;
    int is_palindrome, vowelCount[5] = {0};  // vowel count for a, e, i, o, u

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("Server is running and waiting for messages...\n");

    while (1) {
        addr_len = sizeof(client_addr);
        memset(buffer, 0, MAX_LEN);

        // Receive message from the client
        int n = recvfrom(sockfd, buffer, MAX_LEN, 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0';

        // If the message is "Halt", terminate the server
        if (strcmp(buffer, "Halt") == 0) {
            printf("Server terminating...\n");
            break;
        }

        // Check if the string is a palindrome
        is_palindrome = isPalindrome(buffer);

        // Count occurrences of vowels
        memset(vowelCount, 0, sizeof(vowelCount));
        countVowels(buffer, vowelCount);

        // Prepare the response message
        char result[512];
        sprintf(result, "Palindrome: %s\nLength: %d\nOccurrences of vowels: A: %d, E: %d, I: %d, O: %d, U: %d",
                is_palindrome ? "Yes" : "No", strlen(buffer), vowelCount[0], vowelCount[1], vowelCount[2], vowelCount[3], vowelCount[4]);

        // Send the result back to the client
        sendto(sockfd, result, strlen(result), 0, (const struct sockaddr *)&client_addr, addr_len);
    }

    // Close the socket
    close(sockfd);
    return 0;
}

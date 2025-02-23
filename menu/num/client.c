#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Server port to connect to

int main() {
    int sockfd;
    int ans1, ans2, ans, ans3;
    struct sockaddr_in server_addr;
    char buffer[1024];  // Buffer to store server response
    int num1, num2, num, num3;
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
    while(1){
        int choice;
        printf("MENU:\n1.GCD\n2.LCM\n3.ARMSTRONG\n4.PALINDROME\n5.EXIT\n");
        printf("enter yo choice: \n");
        scanf("%d", &choice);
        send(sockfd, &choice, sizeof(choice),0);
        if(choice==5){
            printf("Client requested exit. Closing connection...\n");
            break;
        }
        if(choice ==1){
            printf("enter number 1: ");
            scanf("%d", &num1);
            printf("enter number 2: ");
            scanf("%d", &num2);
            send(sockfd, &num1, sizeof(num1),0);
            send(sockfd, &num2, sizeof(num2),0);
            recv(sockfd, &ans1, sizeof(int),0);
            printf("GCD IS : %d\n", ans1);
            

        }       
        if(choice==2){
            printf("enter number 1: ");
            scanf("%d", &num1);
            printf("enter number 2: ");
            scanf("%d", &num2);
            send(sockfd, &num1, sizeof(num1),0);
            send(sockfd, &num2, sizeof(num2),0);
            recv(sockfd, &ans2, sizeof(int),0);
            printf("LCM IS : %d\n", ans2);   

        }
        if(choice==3){

            printf("enter a number: ");
            scanf("%d", &num);
            send(sockfd, &num, sizeof(num),0);
            int x=recv(sockfd, &ans, sizeof(ans),0);
            if(ans==1){
                printf("YES , ITS ARMSTRONG\n");
            }
            else{
                printf("NO, NOT ARMSTRONG\n");
            }

        }
        if(choice==4){

            printf("enter a number: ");
            scanf("%d", &num3);
            send(sockfd, &num3, sizeof(num3),0);
            int y=recv(sockfd, &ans3, sizeof(ans3),0);
            if(ans3==1){
                printf("YES , ITS PALINDROME\n");
            }
            else{
                printf("NO, NOT PALINDROME\n");
            }
            
        }       
             
        
    }
    // Close the socket
    close(sockfd);

    return 0;
}

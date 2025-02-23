#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX_SIZE 550
#define PORT 1234  // Define the port on which the server will listen
void rev(char *str){
    int n= strlen(str);
    for(int i=0;i<n/2;i++){        
        char temp=str[i];
        str[i]=str[n-i-1];
        str[n-i-1]=temp;
    }
}
void sort(char *str){
    int n= strlen(str);
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(str[j]>str[j+1]){
                char temp=str[j];
                str[j]=str[j+1];
                str[j+1]=temp;
            }            
        }
    }
}
int  sub(char *str, char *sub){
    if(strstr(str, sub)!=NULL){
        return 1;
    }
    return 0;

}
int vowels(char *str) {
    int count = 0;
    int n = strlen(str);  // Get the length of the string

    for (int i = 0; i < n; i++) {
        char ch = tolower(str[i]);  // Convert to lowercase
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            count++;  // Increase count if it's a vowel
        }
    }
    return count;
}

int main() {
    int server_fd, new_sock;
    int choice;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char str[MAX_SIZE];
    char str1[MAX_SIZE];
    char str2[MAX_SIZE];

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
    while(1){
        recv(new_sock, &choice, sizeof(choice), 0);
        if(choice==5){
            printf("im exiting too\n");
            break;
        }
        if(choice==1){
            int bytes=recv(new_sock, str, MAX_SIZE,0);
            if(bytes<=0){
                printf("connection closed by client\n");
                break;    
            }
            if(strcmp(str, "exit")==0){
                break;
            }
            rev(str);
            send(new_sock, str, strlen(str)+1,0);
        }
        if (choice==2){

            int bytes=recv(new_sock, str, MAX_SIZE,0);
            if(bytes<=0){
                printf("connection closed by client\n");
                break;    
            }
            if(strcmp(str, "exit")==0){
                break;
            }
            sort(str);
            send(new_sock, str, strlen(str)+1,0);
            printf("done sorting\n");


        }
        if(choice==3){
            int byte1= recv(new_sock, str1, MAX_SIZE, 0);
            int byte2= recv(new_sock, str2, MAX_SIZE, 0);
            if(byte1<=0){
                printf("connection closed by client\n");
                break;    
            }
            if(strcmp(str1, "exit")==0){
                break;
            }
            int x= sub(str1, str2);
            send(new_sock, &x, sizeof(int), 0);
            printf("done \n");
        }
        if(choice==4){
            int byte=recv(new_sock, str, MAX_SIZE, 0);
            if(byte<=0){
                printf("connection closed\n");
                break;

            }
            if(strcmp(str, "exit")==0){
                break;
            }
            int c= vowels(str);
            send(new_sock, &c, sizeof(int),0);

        }
        }
        
   
    // Close the socket
    close(new_sock);
    close(server_fd);

    return 0;
}

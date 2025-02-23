#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 1234  // Define the port on which the server will listen
int gcd(int a, int b){
    int temp;
    while(b!=0){
        temp=b;
        b=a%b;
        a=temp;

    }return a;
}
int lcm(int a, int b){
    int gcdd= gcd(a, b);
    return (a*b)/(gcdd);
}
int isarm(int a){
    int x=a;
    int dig;
    int sum=0;
    while(a!=0){
        dig=a%10;
        sum+=dig*dig*dig;
        a/=10;
    }
    if(sum==x){
        return 1;
    }return -1;
}
int ispali(int a){
    int rev=0;
    int x=a;
    while(a!=0){
        int dig= a%10;
        rev=rev*10+dig;
        a/=10;
    }
    if(x==rev){
        return 1;
    }
    return -1;
}
int main() {
    int num1, num2, num, num3;
    int choice;
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
    while(1){
        recv(new_sock, &choice, sizeof(choice),0);
        if(choice==5){
            printf("exiting");
            break;
        }
        if(choice==1){
            recv(new_sock, &num1, sizeof(num1),0);
            recv(new_sock, &num2, sizeof(num2),0);
            int gcdv= gcd(num1,num2);
            send(new_sock, &gcdv, sizeof(int),0);
            printf("message sent to client\n");

        }
        if(choice==2){
            recv(new_sock, &num1, sizeof(num1),0);
            recv(new_sock, &num2, sizeof(num2),0);
            int lcmv= lcm(num1,num2);
            send(new_sock, &lcmv, sizeof(int),0);
            printf("message sent to client\n");

        }
        if(choice==3){
            recv(new_sock, &num, sizeof(num),0);
            int y= isarm(num);
            send(new_sock, &y, sizeof(int), 0);
            printf("message sent to client\n");

        }
        if(choice==4){
            recv(new_sock, &num3, sizeof(num3),0);
            int x= ispali(num3);
            send(new_sock, &x, sizeof(int), 0);
            printf("message sent to client\n");

        }
        
    }
    // Close the socket
    close(new_sock);
    close(server_fd);

    return 0;
}

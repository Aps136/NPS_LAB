#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 1234  // Define the port on which the server will listen
void handle_client(int client_sock){
    char filename[256];
    FILE *file;
    recv(client_sock, filename, sizeof(filename),0);
    filename[strcspn(filename, "\n")]=0;
    file = fopen(filename,"r");
    if(!file){
        char msg[]="file not found";
        send(client_sock, msg, sizeof(msg),0);
        close(client_sock);
        return;
    }
    int wcount=0;
    char ch, prev=' ';
    while((ch=fgetc(file))!=EOF){
        if((ch==' '||ch=='\n'||ch=='\t')&& (prev!=' '&&prev!='\n'&&prev!='\t')){
            wcount++;
        }
        prev=ch;
    }
    fclose(file);
    char response[50];
    sprintf(response, "word count : %d", wcount);
    send(client_sock, response, sizeof(response), 0);
    close(client_sock);
}


int main() {
    int client_sock;
    int server_fd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size=sizeof(client_addr);
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

    while(1){
        client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
        if(client_sock<0){
            perror("accept failed");
            continue;
        }
        handle_client(client_sock);
    }
    close(server_fd);

    return 0;
}

#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>

main(){
    int s,r,recb,sntb,x;
    printf("INPUT port number: ");
    scanf("%d",&x);
    socklen_t len;
    struct sockaddr_in server;
    char buff[50];
    s = socket(AF_INET,SOCK_STREAM,0);
    if(s==1){
        printf("\nSocket creation error. ");
        exit(0);
    }
    printf("\nSocket created.");
    server.sin_family = AF_INET;
    server.sin_port = htons(x);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    r = connect(s,(struct sockaddr*)&server,sizeof(server));
    if(r==-1){
        printf("\nConnection error.");
        exit(0);
    }
    printf("\nSocket connected.");
    printf("\n\n");

    printf("Input Domain Name: ");
    scanf("%s",buff);
    sntb = send(s,buff,sizeof(buff),0);
    recb = recv(s,buff,sizeof(buff),0);
    printf("\nMessage Recieved: %s\n",buff);

    close(s);
}

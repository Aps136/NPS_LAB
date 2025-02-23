#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>

main(){
    int s,r,recb,sntb,x,ns,a=0;

    printf("INPUT port number: ");
    scanf("%d",&x);
    socklen_t len;
    struct sockaddr_in server ,client;
    char buff[50];
    s = socket(AF_INET,SOCK_STREAM,0);
    if(s==1){
        printf("\nSocket creation error. ");
        exit(0);
    }
    printf("\nSocket created.");
    server.sin_family = AF_INET;
    server.sin_port = htons(x);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(client);
    r = bind(s,(struct sockaddr*)&server,sizeof(server));
    if(r==-1){
        printf("\nBinding error.");
        exit(0);
    }
    printf("\nSocket binded.");
    r = listen(s,1);
    if(r==-1){
        close(s);
        exit(0);
    }
    printf("\nSocket listening.");
    ns = accept(s,(struct sockaddr*)&client,&len);
    if(ns==-1){
        close(s);
        exit(0);
    }
    printf("\nSocket accepting.");

    FILE *fptr = fopen("DataBase.txt","r");
    if(!fptr){
        printf("Error");
        close(ns);
        exit(0);
    }

    
    int found = 0;
    char line[100];
    recb = recv(ns,buff,sizeof(buff),0);
    printf("\nMessage Recieved: %s\n",buff);

    while(fgets(line,100,fptr)){
        line[strlen(line)-1] = '\0';
        if(strcmp(line,buff)==0){
            fgets(buff,50,fptr);
            found = 1;
            break;
        }
    }
    if(!found)
        strcpy(buff,"Domain does not exist");
    sntb = send(ns,buff,sizeof(buff),0);
        
    fclose(fptr);
    close(ns);
    exit(0);
}

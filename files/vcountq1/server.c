#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <ctype.h>
#define MAXSIZE 90
int handle(int newsockfd){
    char filename[550];
    FILE *file;
    recv(newsockfd, filename, sizeof(filename),0);
    filename[strcspn(filename,"\n")]=0;
    file = fopen(filename,"r");
    if(!file){
        char msg[]="file not found";
        send(newsockfd, msg, sizeof(msg),0);
        close(newsockfd);
        return -1;}
    
    int c=0;
    char ch;
    while((ch=fgetc(file))!=EOF){
        char x= tolower(ch);
        if(x=='a'||x=='e'||x=='i'||x=='o'||x=='u'){
            c++;
        }
    }fclose(file);
    send(newsockfd, &c, sizeof(c),0);
    close(newsockfd);
    return 0;
    }


int main()
{
int sockfd,newsockfd,retval;
socklen_t actuallen;
int recedbytes,sentbytes;
struct sockaddr_in serveraddr,clientaddr;

char buff[MAXSIZE];
int a=0;
sockfd=socket(AF_INET,SOCK_STREAM,0);

if(sockfd==-1)
{
printf("\nSocket creation error");
}

serveraddr.sin_family=AF_INET;
serveraddr.sin_port=htons(3388);
serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
if(retval==1)
{
printf("Binding error");
close(sockfd);
}

retval=listen(sockfd,1);
if(retval==-1)
{
close(sockfd);
}
while(1){
    actuallen=sizeof(clientaddr);
    newsockfd= accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
    if(newsockfd==-1)
    {
    close(sockfd);
    }
    handle(newsockfd);


}

close(sockfd);
close(newsockfd);
return 0;
}


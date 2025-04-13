#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include <ctype.h>
#define MAXSIZE 50

int main()
{
int sockfd,retval;
int recedbytes,sentbytes;
struct sockaddr_in serveraddr;
char buff[MAXSIZE];
char filename[550];
sockfd=socket(AF_INET,SOCK_STREAM,0);
if(sockfd==-1)
{
printf("\nSocket Creation Error");

}
printf("%i",sockfd);
serveraddr.sin_family=AF_INET;
serveraddr.sin_port=htons(3388);
serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
if(retval==-1)
{
printf("Connection error");
close(sockfd);
exit(1);

}
int x;
printf("enter filename\n");
scanf("%s",filename);
send(sockfd,filename,sizeof(filename)+1,0);
printf("sent filename\n");
recv(sockfd,&x,sizeof(x),0);
if(x==-1){
    printf("file not found\n");
}else{
    printf("vowel count %d\n",x);
}




close(sockfd);
return 0;
}

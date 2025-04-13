#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include <ctype.h>
#define MAXSIZE 200

main()
{
	char buff[MAXSIZE];
	int sockfd,retval,i;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
		return;
	}

	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
		return;
	}
    if(fork()==0){
        while(1){
            //child->receive
            memset(buff, 0, sizeof(buff));
            if(recv(sockfd, buff, sizeof(buff), 0)<=0) break;
            if(strncmp(buff,"stop",4)==0) break;
            printf("server:%s\n", buff);
        }
    }else{
        while(1){
            memset(buff, 0, sizeof(buff));
            printf("you: ");
            fgets(buff, sizeof(buff),stdin);
            buff[strcspn(buff,"\n")]='\0';
            send(sockfd, buff, strlen(buff)+1, 0);
            if(strncmp(buff, "stop",4)==0) break;

        }
    }
    close(sockfd);
    return 0;

	
}

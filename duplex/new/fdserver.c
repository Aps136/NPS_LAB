#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAXSIZE 200

main()
{
	int sockfd,newsockfd,retval,i;
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
	actuallen=sizeof(clientaddr);
	newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
	if(newsockfd==-1)
	{
		close(sockfd);
	}
	
    if(fork()==0){
        //child process ->receive
        while(1){
            memset(buff, 0,sizeof(buff));
            if(recv(newsockfd, buff, sizeof(buff),0)<=0) break;// if didnt get data
            if(strncmp(buff, "stop",4)==0) break;//if it says stop
            printf("client: %s\n",buff);
        }
    }else{
        //parent->send data
        while(1){
            memset(buff, 0, sizeof(buff));
            printf("You:");
            fgets(buff, sizeof(buff), stdin);
            buff[strcspn(buff,"\n")]='\0';
            send(newsockfd, buff, strlen(buff)+1,0);
            if(strncmp(buff, "stop",4)==0) break;

        }
    }close(newsockfd);
    close(sockfd);
    return 0;
	
}


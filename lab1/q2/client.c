#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#define max 50
int main()
{
	int sockfd,retval,newsockfd,k=0;
	socklen_t actuallen;
	char c;
	char temp[max];
    char result[max];
	size_t leng;
	FILE *fp;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[max];
	char temp1[max];
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		printf("\n Socket creation error");
		exit(0);
	}
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_port=htons(3201);
	clientaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3200);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=bind(sockfd,(struct sockaddr *)&clientaddr,sizeof(clientaddr));
	if(retval==-1)
	{
		printf("\n Binding error");
		close(sockfd);
	 	exit(0);
	}
	
	actuallen=sizeof(serveraddr);
	while(1){
        printf("enter string: ");
        fgets(buff, max, stdin);
        buff[strcspn(buff,"\n")]='\0';
        if(strcmp(buff,"exit")==1){
            printf("ok byeee\n");
            break;


        }
        sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&serveraddr,actuallen);
        recvfrom(sockfd,result,sizeof(result),0,(struct sockaddr *)&serveraddr,&actuallen);
        printf("server says : %s\n",result);
       
       
    }
    close(sockfd);
    
    return 0;

	
	

}


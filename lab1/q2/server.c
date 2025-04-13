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
int ispali(char str[]){
    int i=0;
    int len=0;
    while(str[len]!='\0'){
        len++;
    }
    for(int i=0;i<len/2;i++){
        if(str[i]!=str[len-i-1]){
            return 0;
        }
    }return 1;
}
int main()
{
	int sockfd,newsockfd,k;
	socklen_t actuallen;
	int retval;
	size_t leng;
	char c;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[max],temp[max],result[max];
	int a=0;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		printf("\n Socket creation error");
		exit(0);
	}
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3200);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_port=htons(3201);
	clientaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	retval=bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("\n Binding error");
		close(sockfd);
	 	exit(0);
	}
    actuallen=sizeof(clientaddr);
    while(1){
        
    recvfrom(sockfd, temp,sizeof(temp),0,(struct sockaddr *)&clientaddr, &actuallen);
    if(strcmp(temp,"exit")==1){
        printf("bye");
        break;
    }
	printf("Received string : %s\n",temp);
    if(ispali(temp)){
        strcpy(result, "Palindrome");

    }else{
        strcpy(result, "not palindrome");

    }


	sendto(sockfd,result,sizeof(result),0,(struct sockaddr *)&clientaddr,actuallen);
	
    }
	close(sockfd);
    return 0;
}


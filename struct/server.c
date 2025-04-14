#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#define MAXSIZE 90
struct student{
    char name[50];
    char usn[20];
    char branch[30];
    int sem;

};
struct student db[10];
int count=0;
void insert(struct student s){

    db[count++]=s;
    printf("student inserted\n");
}
void search(char usn[]){
    int found=0;
    for(int i=0;i<count;i++){
        if(strcmp(db[i].usn,usn)==0){
            printf("name:%s\nusn:%s\nbrnach:%s\nsem:%d\n",db[i].name,db[i].usn,db[i].branch,db[i].sem);
            found=1;
            break;
            
        }if(!found) printf("not found\n");
    }
}
void update(char usn[], struct student s){

    for(int i=0;i<count;i++){
        if(strcmp(db[i].usn,usn)==0){
                db[i]=s;
                printf("rec updated\n");
                return;
        }
    }printf("usn not found\n");
}
void del(char usn[]){
    int found=0;
    for(int i=0;i<count;i++){
        if(strcmp(db[i].usn,usn)==0){
            for(int j=i;j<count-1;j++){
                db[j]=db[j+1];
            }count--;
            printf("rec del\n");
            found=1;
            break;
        }
    }if(!found) printf("usn not found\n");
}
void disp(){
    for(int i=0;i<count;i++){
        printf("name:%s\nusn:%s\nbrnach:%s\nsem:%d\n",db[i].name,db[i].usn,db[i].branch,db[i].sem);
    }
}
main()

{
struct student s;   
char usn[20];

int sockfd,newsockfd,retval;
socklen_t actuallen;
int recedbytes,sentbytes;
struct sockaddr_in serveraddr,clientaddr;
int choice;

sockfd=socket(AF_INET,SOCK_STREAM,0);

if(sockfd==-1)
{
printf("\nSocket creation error");
exit(1);
}

serveraddr.sin_family=AF_INET;
serveraddr.sin_port=htons(4000);
serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
if(retval==-1)
{
printf("Binding error");
close(sockfd);
exit(1);
}

retval=listen(sockfd,1);
if(retval==-1)
{
close(sockfd);
exit(1);
}

actuallen=sizeof(clientaddr);
newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
while(1){
    recv(newsockfd,&choice, sizeof(choice),0);
    if(choice==1){
        recv(newsockfd,&s, sizeof(s),0);
        insert(s);

    }else if(choice==2){
        recv(newsockfd,&usn, sizeof(usn),0);
        search(usn);
    }
    else if(choice==3){
        recv(newsockfd,&usn, sizeof(usn),0);
        recv(newsockfd,&s, sizeof(s),0);
        update(usn,s);
    }
    else if(choice==4){
        recv(newsockfd,&usn, sizeof(usn),0);
        del(usn);
    }else if(choice==5){
        disp();
    }else{
        break;
    }

}

close(newsockfd);
close(sockfd);

}


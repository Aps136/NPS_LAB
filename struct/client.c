#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#define MAXSIZE 50
struct student{
    char name[50];
    char usn[20];
    char branch[30];
    int sem;

};
main()
{
int choice;

struct student s;
int sockfd,retval;
int recedbytes,sentbytes;
struct sockaddr_in serveraddr;
char buff[MAXSIZE];
sockfd=socket(AF_INET,SOCK_STREAM,0);
if(sockfd==-1)
{
printf("\nSocket Creation Error");
exit(1);

}
printf("%i",sockfd);
serveraddr.sin_family=AF_INET;
serveraddr.sin_port=htons(4000);
serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
if(retval==-1)
{
printf("Connection error");
exit(1);

}
while(1){
    printf("MENU:\n1.add new rec\n2.search by sname\n3.update deets by name\n4.delete\n5.disp\n6.exit\n");
    printf("enter yo choice\n");
    scanf("%d",&choice);
    char usn[20];
    send(sockfd, &choice, sizeof(choice),0);
    if(choice==6){
        printf("client says bye...\n");
        break;

    }else if(choice==1){
        printf("enter student name: ");
        scanf(" %[^\n]",s.name);
        printf("enter student username: ");
        scanf(" %[^\n]",s.usn);
        printf("enter student branch: ");
        scanf(" %[^\n]",s.branch);
        printf("enter student sem: ");
        scanf("%d",&s.sem);
        send(sockfd, &s, sizeof(s),0);
        printf("sent from client\n");

    }
    else if(choice==2){
        printf("enter name to search deets: ");
        scanf(" %[^\n]",usn);
        send(sockfd, &usn, sizeof(usn),0);
        
    }else if(choice==3){
        printf("enter name to update deets: ");
        scanf(" %[^\n]",usn);
        send(sockfd,&usn, sizeof(usn),0);
        printf("Enter New Name: "); scanf(" %[^\n]", s.name);
        printf("Enter New USN: "); scanf(" %[^\n]", s.usn);
        printf("Enter New Branch: "); scanf(" %[^\n]", s.branch);
        printf("Enter New Semester: "); scanf("%d", &s.sem);
        send(sockfd, &s, sizeof(s), 0);
        
    }
    else if(choice==4){
        printf("enter name to delete deets: ");
        scanf(" %[^\n]",usn);
        send(sockfd, &usn, sizeof(usn),0);
        
    }else if(choice==5){
       
        
    }

}

close(sockfd);
}

#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
int main()
{
	int sockdesc;
	struct sockaddr_in servaddr;
	sockdesc=socket(AF_INET,SOCK_DGRAM,0);
	if(sockdesc==-1)
	{
		printf("Socket not created");
		return -1;
	}

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(11164);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	int n;
	char temp;
	printf("Number of commands?\n");
	scanf("%d",&n);
	scanf("%c",&temp);
	for(int i=0;i<n;i++)
	{
		char buffer[20]="";
		char buf[20]="";
		char ans[20]="";
		printf("Enter the command: ");
		scanf("%s",buffer);
		printf("Enter the input: ");
		scanf("%s",buf);
		sendto(sockdesc,buffer,strlen(buffer),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
		sendto(sockdesc,buf,strlen(buf),0,(struct sockaddr *)&servaddr,sizeof(servaddr));
		int len=sizeof(servaddr);
		recvfrom(sockdesc,ans,sizeof(ans),0,(struct sockaddr *)&servaddr,&len);
		printf("Message from server: %s\n", ans);
	}


	close(sockdesc);
	return 0;

}

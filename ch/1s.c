#include<stdio.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
int main()
{
	int sockdesc;
	struct sockaddr_in servaddr,cliaddr;

	sockdesc=socket(AF_INET,SOCK_DGRAM,0);
	if(sockdesc==-1)
	{
		printf("Socket not created");
		return -1;
	}

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(11064);                 // PORT number ranges from 1024 to 49151
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(sockdesc,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("Bind Failed");
		return -1;
	}

	while(1)
	{
		int len=sizeof(cliaddr);

		printf("Waiting for a request...\n");

		char buffer[10]="";
		char buf[10]="";
		char ans[20]="";
		//strcpy(buffer," ");
		//strcpy(ans," ");
		recvfrom(sockdesc,buffer,sizeof(buffer),0,(struct sockaddr *)&cliaddr,&len);
		recvfrom(sockdesc,buf,sizeof(buf),0,(struct sockaddr *)&cliaddr,&len);
		printf("Message received from client:\n%s\n%s", buffer, buf);
		if(strcmp(buffer,"EVENODD")==0)
		{
			int eo= atoi(buf);
			if(eo%2==0)
				ans[0]='E';
			else
				ans[0]='O';
		}

		sendto(sockdesc,ans,strlen(ans),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

	}

	close(sockdesc);



	return 0;
}

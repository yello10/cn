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

	sockdesc=socket(AF_INET,SOCK_STREAM,0);
	if(sockdesc==-1)
	{
		printf("Socket not created");
		return -1;
	}

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(6133);                 // PORT number ranges from 1024 to 49151
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(sockdesc,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("Bind Failed");
		return -1;
	}

	if(listen(sockdesc,5)<0)
	{
		printf("Listen Failed");
		return -1;
	}


	while(1)
	{
		int len=sizeof(cliaddr);
		int connfd=accept(sockdesc,(struct sockaddr*)&cliaddr,&len);
		if (connfd<0)
		{
			printf("Accept failed");
			return -1;
		}


		char buffer[40];
		char buffer2[50]=" ";
		int j=0, ct=0;
		strcpy(buffer," ");
		strcpy(buffer2," ");
		read(connfd,buffer,40);
		printf("Input from user is %s", buffer);
		for(int i=0; buffer[i]!='\0'; i++)
		{
			if(buffer[i]=='0')
			{
				buffer2[j]='0';
				j++;
				ct=0;
			}
			else if(buffer[i]=='1')
			{
				buffer2[j]='1';
				ct++;
				j++;
				if(ct==5)
				{
					buffer2[j]='0';
					j++;
					ct=0;
				}
			}
		}
		write(connfd,buffer2,sizeof(buffer2));
		printf("answer is %s\n",buffer2);
		strcpy(buffer2," ");
		strcpy(buffer," ");
		printf("new strings are %s AND %s \n",buffer,buffer2);

	}

	close(sockdesc);



	return 0;
}

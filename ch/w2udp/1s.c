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
	servaddr.sin_port=htons(20009);                 // PORT number ranges from 1024 to 49151
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

		char buffer[20]="";
		char buf[20]="";
		char ans[20]="";
		//strcpy(buffer," ");
		//strcpy(ans," ");
		recvfrom(sockdesc,buffer,sizeof(buffer),0,(struct sockaddr *)&cliaddr,&len);
		recvfrom(sockdesc,buf,sizeof(buf),0,(struct sockaddr *)&cliaddr,&len);
		printf("Message received from client:\nCommand:%s\nInput:%s\n", buffer, buf);
		if(strcmp(buffer,"EVENODD")==0)
		{
			int flag=1;
			for(int i=0; i<strlen(buf); i++)
			{
				if(buf[i]<'0' || buf[i]>'9')
				{
					flag=-1;
					break;
				}
			}
			if(flag!=-1)
			{
				int eo= atoi(buf);
				if(eo%2==0)
					strcpy(ans,"Even Number");
				else if(eo%2!=0)
					strcpy(ans,"Odd Number");
			}
			else if(flag==-1)
				strcpy(ans,"Invalid input");
			//printf("\n%c\n",ans[0]);
		}
		else if(strcmp(buffer,"PALINDROME")==0)
		{
			int flag=1;
			int leng=strlen(buf);
			int j=leng-1;
			for(int i=0; i<leng/2; i++,j--)
			{
				if(buf[i]!=buf[j])
				{
					flag=-1;
					break;
				}
			}
			if(flag==-1)
				strcpy(ans,"Not Palindrome");
			else
				strcpy(ans,"Palindrome");
		}
		else if(strcmp(buffer,"NUM01")==0)
		{
			int flag=1,ct1=0,ct0=0;
			for(int i=0; i<strlen(buf); i++)
			{
				if(buf[i]=='1')
					ct1++;
				else if(buf[i]=='0')
					ct0++;
				else
				{
					flag=-1;
					break;
				}
			}
			if(flag==-1)
				strcpy(ans,"Invalid input");
			else
			{
				ans[0]=ct1+48;
				ans[1]=' ';
				ans[2]=ct0+48;
			}
		}
		else
		{
			strcpy(ans,"Invalid command");
		}
		sendto(sockdesc,ans,strlen(ans),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

	}

	close(sockdesc);



	return 0;

}

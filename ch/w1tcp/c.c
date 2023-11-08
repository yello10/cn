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
	sockdesc=socket(AF_INET,SOCK_STREAM,0);
	if(sockdesc==-1)
	{
		printf("Socket not created");
		return -1;
	}

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(1575);			
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

	if (connect(sockdesc,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("Connect Failed");
		return -1;
	}

	char buffer[2000];
	fgets(buffer,sizeof(buffer),stdin);
	write(sockdesc,buffer,sizeof(buffer));

	read(sockdesc,buffer,10);
	printf("Message from server: %s", buffer);


	close(sockdesc);
	return 0;

}

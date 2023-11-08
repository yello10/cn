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
	servaddr.sin_port=htons(12250);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);

	char buffer[50] = "";
	printf("Enter the domain name: ");
	scanf("%s", buffer);
	sendto(sockdesc,buffer,strlen(buffer),0,(struct sockaddr *)&servaddr,sizeof(servaddr));

	int len=sizeof(servaddr);

	char output[50] = "";
	recvfrom(sockdesc,output,sizeof(output),0,(struct sockaddr *)&servaddr,&len);
	printf("IP address: %s\n", output);


	close(sockdesc);
	return 0;

}

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
	servaddr.sin_port=htons(22222);                  // PORT number ranges from 1024 to 49151
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

		int buffer[10];
		recvfrom(sockdesc,buffer,sizeof(buffer),0,(struct sockaddr *)&cliaddr,&len);
		int i = 2,n = buffer[1],total[1];
		total[0] = 0;
		while(n > 0){
			printf("%d ", buffer[i]);
			total[0] += buffer[i];
			i++;
			n--;
		}
		printf("\nAddition: %d\n", total[0]);

		sendto(sockdesc,total,sizeof(total),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

	}

	close(sockdesc);



	return 0;
}

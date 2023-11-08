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
	servaddr.sin_port=htons(12250);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(sockdesc,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("Bind Failed");
		return -1;
	}

	char output[50] = "";
	char *dom[5] = {"vit.ac.in", "google.com", "ieee.org", "annauniv.edu", "yahoo.co.in"};
	char *ip[5] = {"172.178.1.209", "100.100.1.100", "123.456.7.890", "172.168.1.200", "174.123.1.300"};
	while(1)
	{
		int len=sizeof(cliaddr);

		printf("Waiting for a request...\n");

		char buffer[50] = "";
		recvfrom(sockdesc,buffer,sizeof(buffer),0,(struct sockaddr *)&cliaddr,&len);
		printf("Message received from client: %s\n", buffer);

		for (int i=0; i<5; i++)
		{
			if (strcmp(dom[i], buffer) == 0)
				strcpy(output, ip[i]);
		}

		printf("output: %s\n\n", output);
		sendto(sockdesc,output,strlen(output),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));

	}

	close(sockdesc);



	return 0;
}

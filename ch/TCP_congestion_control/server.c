#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256
int main(int argc, char* argv[])
{
	int rand_handle=0;
	if (argc==2) 
	{
		rand_handle = atoi(argv[1]);
	}
	else 
	{
		fprintf(stderr, "usage:<rand_handle>: 0 for enabling rand packet drop and 1 otherwise\n");
		exit(1);
	}
	struct sockaddr_in sin,client;
	socklen_t len;
	int s, new_s;
	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);
	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) 
	{
		perror("simplex-talk: bind");
		exit(1);
	}
	int* pack_arrival=(int *)malloc(1000*MAX_LINE*sizeof(int));
	int pack_arrival_count=0;
	/*create a buffer for receiving packets*/
	char* received_buffer=(char *)malloc(17*sizeof(char));
	/*create a buffer for sending ACK*/
	char* ack=(char *)malloc(9*sizeof(char));
	/*establish TCP connection by using a 3-way handshake: SYN(server to client), SYN-ACK(client to server), ACK(server to client)*/
	char* dummy=(char *)malloc(17*sizeof(char));
	char* syn_ack=(char *)malloc(17*sizeof(char));
	char* tcp3=(char *)malloc(17*sizeof(char));
	char* syn=(char *)malloc(9*sizeof(char));
	syn="88888888SYN\0";
	tcp3="88888888tcp3\0";
	recvfrom(s, dummy, 17*sizeof(dummy), 0,(struct sockaddr *) &client, &len);
	sendto(s, syn, strlen(syn)+1, 0,(struct sockaddr *) &client, len);/*sending SYN to the client*/
	printf("%s\n","Sent     >> SYN" );
	if(recvfrom(s, syn_ack, 17*sizeof(syn_ack), 0,(struct sockaddr *) &client, &len)>0)/*receiving SYN-ACK from client*/
	printf("%s\n","Received << SYN-ACK");
	sendto(s, tcp3, strlen(tcp3)+1, 0,(struct sockaddr *) &client, len);/*sending tcp3 to the client*/
	printf("%s\n","Sent     >> Request for Seqnum: 00000000" );
	/*creating a buffer to read file data*/
	char* read_buffer=(char *)malloc(10000*MAX_LINE*sizeof(char));
	int read_buffer_count=0;
	int while_count=0;
	int total_packets=atoi(syn_ack);
	int recv_pack_count=0;
	int flag2=0;
	int expected_seqnum=0;
	while(1)
	{
				if(recv_pack_count>=total_packets)
				{
					if(recvfrom(s, received_buffer, 17*sizeof(received_buffer), 0,(struct sockaddr *) &client, &len)>0)
					{
						pack_arrival[pack_arrival_count]=time(NULL);
						pack_arrival_count++;
						/*checking for fin packet and break*/
						int flag=1;
						for(int i=0;i<8;i++)
						{
							if(received_buffer[i]!='9')
							{
								flag=0;
								break;
							}

						}
						if(flag==1)
						{
							//flag2=1;
							break;
						}
			   			printf("%s","Received << Seqnum 	: " );
						for(int i=0;i<8;i++)
						{
							sprintf(&ack[i],"%s",&received_buffer[i]);/*writing ACK into the buffer*/
							printf("%c",received_buffer[i] );
						}
						ack[8]='\0';
						printf("\n");
						int check=atoi(ack);
						if(expected_seqnum==check)
						{
							for(int i=0;i<8;i++)
							{
								read_buffer[read_buffer_count]=received_buffer[i+8];
								read_buffer_count++;
							}
							sendto(s, ack, strlen(ack)+1, 0,(struct sockaddr *) &client, len);/*sending ACK to the client*/
							printf("%s","Sent     >> ACK 	: " );
							printf("%s\n",ack );
							recv_pack_count++;
							expected_seqnum++;
						}
					}
				}
				int rand_num=rand()%10;
				if(rand_num<4||rand_handle)/*if rand_num is less than 7, allow packets; or if rand_handle is 0, no dropping*/
				{
					if(recvfrom(s, received_buffer, 17*sizeof(received_buffer), 0,(struct sockaddr *) &client, &len)>0)
					{
						pack_arrival[pack_arrival_count]=time(NULL);
						pack_arrival_count++;
						/*checking for fin packet and break*/
						int flag=1;
						for(int i=0;i<8;i++)
						{
							if(received_buffer[i]!='9')
							{
								flag=0;
								break;
							}

						}
						if(flag==1)
						{
							//flag2=1;
							break;
						}
			   			printf("%s","Received << Seqnum 	: " );
						for(int i=0;i<8;i++)
						{
							sprintf(&ack[i],"%s",&received_buffer[i]);/*writing ACK into the buffer*/
							printf("%c",received_buffer[i] );
						}
						ack[8]='\0';
						printf("\n");
						int check=atoi(ack);
						if(expected_seqnum==check)
						{
							for(int i=0;i<8;i++)
							{
								read_buffer[read_buffer_count]=received_buffer[i+8];
								read_buffer_count++;
							}
							sendto(s, ack, strlen(ack)+1, 0,(struct sockaddr *) &client, len);/*sending ACK to the client*/
							printf("%s","Sent     >> ACK 	: " );
							printf("%s\n",ack );
							recv_pack_count++;
							expected_seqnum++;
						}
					}
				}
				else/*if rand_num is greater than or equal to 7, drop packets*/
				{
					printf("%s\n","Packet Dropped" );
					usleep(50000);
				}	
	}
	FILE *packptr;
	FILE *countptr;
	if ((packptr=fopen("pack_arrival.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<pack_arrival_count;i++)
	{
		fprintf(packptr,"%d\n",pack_arrival[i]);
	}
	fclose(packptr);
	if ((countptr=fopen("pack_arrival_count.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<pack_arrival_count;i++)
	{
		fprintf(countptr,"%d\n",i+1);
	}
	fclose(countptr);
	char* read_buffer_string=(char *)malloc(10000*MAX_LINE*sizeof(char));
	for(int i=0;i<read_buffer_count;i++)
	{
		sprintf(&read_buffer_string[i],"%s",&read_buffer[i]);
	}
	read_buffer[strlen(read_buffer)+1]='\0';
	/*open the file and write the packet data to it*/
	FILE *uptr;
	if ((uptr=fopen("out.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<strlen(read_buffer_string);i++)
	{
		fprintf(uptr,"%c",read_buffer[i]);
	}
	fclose(uptr);
	
}


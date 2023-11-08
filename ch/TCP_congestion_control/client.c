#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/time.h>
#include<time.h>
#define SERVER_PORT 5432
#define MAX_LINE 256
int main(int argc, char * argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin,server;
	char *host;
	char buf[MAX_LINE];
	int s;
	int new_s;
	int len;
	if (argc==5) 
	{
		host = argv[1];
	}
	else 
	{
		fprintf(stderr, "usage: <localhost> <filename> <timeout in microseconds (>100000)> <ssthresh>\n");
		exit(1);
	}
	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) 
	{
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}
	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);
	/* active open */
	if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		perror("simplex-talk: socket");
		exit(1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	/*read from file and store it in buffer*/
	FILE *fptr;
	char text;
	char* filename=(char *)malloc( 30*sizeof(char));
	filename=argv[2];
	if ((fptr = fopen(filename/*read_file*/,"r"))==NULL)
    {
       printf("Error! opening file");
       exit(1);
    }
    /*creating a buffer to read file data*/
	char* read_buffer=(char *)malloc(10000*MAX_LINE*sizeof(char));
	int char_count=0;
	/*reading file data into buffer*/
	while((fscanf(fptr,"%c",&text)==1))
	{
		read_buffer[char_count]=text;
		char_count++;
	}
	int total_packets=(char_count/8)+1;
	/*establish TCP connection by using a 3-way handshake: SYN(server to client), SYN-ACK(client to server), ACK/Request for Seqnum: 00000000(server to client)*/
	char* dummy=(char *)malloc(17*sizeof(char));
	char* syn=(char *)malloc(17*sizeof(char));
	char* syn_ack=(char *)malloc(17*sizeof(char));
	char* tcp3=(char *)malloc(17*sizeof(char));
	char* char_packet_number=(char *)malloc(17*sizeof(char));
	int* int_packet_number=(int *)malloc(17*sizeof(int));
	int* ack_arrival=(int *)malloc(1000*MAX_LINE*sizeof(int));
	int ack_arrival_count=0;
	int* ack_lost=(int *)malloc(1000*MAX_LINE*sizeof(int));
	int ack_lost_count=0;
	int* timestamp=(int *)malloc(1000*MAX_LINE*sizeof(int));
	int timestamp_count=0;
	/*buffer for storing congestion_window size*/
	int* congestion_window_buffer=(int *)malloc(1000*MAX_LINE*sizeof(int));
	int congestion_window_count=0;
	int tem=total_packets;
	int h=0;
	while(tem>0)/*writing seqnum (int) to a buffer*/
			{
				int_packet_number[h]=tem%10;
				tem=tem/10;
				h++;
			}
	for(int i=0;i<h;i++)/*writing seqnum (char) to a buffer*/
			{
				sprintf(&char_packet_number[i],"%d",int_packet_number[h-i-1]);
			}
	dummy="88888888Know_my_IP\0";
	syn_ack=char_packet_number;
	int dummy_len=strlen(dummy)+1;
	int syn_ack_len=strlen(syn_ack)+1;
	socklen_t send_len=sizeof(sin);
	sendto(s, dummy/*dynamic_buffer*/, dummy_len, 0,(struct sockaddr *)&sin, send_len);
	if(recvfrom(s, syn/*dynamic_buffer*/,17*sizeof(char), 0,(struct sockaddr *)&sin, &send_len)>0)/*receiving SYN from server*/
	printf("%s\n","Received << SYN");
	sendto(s, syn_ack/*dynamic_buffer*/, syn_ack_len, 0,(struct sockaddr *)&sin, send_len);/*sending SYN-ACK to server*/
	printf("%s\n","Sent >> SYN-ACK");
	if(recvfrom(s, tcp3/*dynamic_buffer*/,17*sizeof(char), 0,(struct sockaddr *)&sin, &send_len)>0)/*receiving request for Seqnum: 00000000*/
	printf("%s\n","Received << Request for Seqnum: 00000000" );
	/************************************************************************/
	/*static packet allocation*/
	int packet_index=0;
	/*congestion window*/
	int congestion_window=1;
	int ssthresh=atoi(argv[4]);
	/*packetize text and send packets*/
	while(packet_index<strlen(read_buffer))
	{
		if(congestion_window==0)
			congestion_window=1;
		printf("%s%d\n","Window Size: ",congestion_window );
		congestion_window_buffer[congestion_window_count]=congestion_window;
		congestion_window_count++;
		for(int l=0;l<congestion_window;l++)/*sending congestion window number of packets*/
		{
			timestamp[timestamp_count]=time(NULL);
			timestamp_count++;
			/*assigning seqnum*/
			int temp=packet_index/8;
			int x=0;
			int* seqnum_buffer=(int *)malloc(9*sizeof(int));
			char* seqnum=(char *)malloc(17*sizeof(char));
			char* seqnum_copy=(char *)malloc(17*sizeof(char));
			while(temp>0)/*writing seqnum (int) to a buffer*/
			{
				seqnum_buffer[7-x]=temp%10;
				temp=temp/10;
				x++;
			}
			x--;
			for(int i=0;i<7-x;i++)
				{
					seqnum_buffer[i]=0;
				}
			for(int i=0;i<8;i++)/*writing seqnum (char) to a buffer*/
			{
				sprintf(&seqnum[i],"%d",seqnum_buffer[i]);
				sprintf(&seqnum_copy[i],"%d",seqnum_buffer[i]);
			}
			seqnum[strlen(seqnum)]='\0';
			char* dynamic_buffer=(char *)malloc(17*sizeof(char));
			for(int i=0;i<8;i++)/*reading packet data to buffer (data is in the form of 8 bits)*/
			{
				dynamic_buffer[i]=read_buffer[packet_index+i];
			}
			strcat(seqnum,dynamic_buffer);/*concatenating seqnum and packet data*/
			len = strlen(seqnum) + 1;
			sendto(s, seqnum/*dynamic_buffer*/, len, 0,(struct sockaddr *)&sin, send_len);/*sending packet to the server*/
			printf("%s%s\n","Sent     >> Seqnum 	: ",seqnum_copy);
			packet_index=packet_index+8;/*updating packet index (the first bit of next packet data)*/
			
		}
		int next_congestion_window=congestion_window;
		for(int l=0;l<congestion_window;l++)/*waiting for corresponding ACKs*/
		{
			timestamp[timestamp_count]=time(NULL);
			timestamp_count++;
		/*seting timeout for recvfrom() socket at 200000 micro seconds*/   
			struct timeval tv;
  			tv.tv_sec=0;
  			tv.tv_usec=atoi(argv[3]);
  			if(setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(const char*)&tv,sizeof(tv))<0)
  			{
  				perror("setsockopt_error");
  			}
			char* ack=(char *)malloc(26*sizeof(char));
			if((/*ack_len=*/recvfrom(s, ack/*dynamic_buffer*/,17*sizeof(char), 0,(struct sockaddr *)&sin, &send_len))<0) /*timeout reached => ACK not received, so resend packet*/
			{
				printf("%s\n","Not Received << ACK ");
				ack_lost[ack_lost_count]=time(NULL);
				ack_lost_count++;
				packet_index=packet_index-(8*(congestion_window-l));
				next_congestion_window=congestion_window/2;
				break;
			}
			else/*receive ACK and print*/
			{
				printf("%s%s\n","Received << ACK 	: ",ack);
				ack_arrival[ack_arrival_count]=time(NULL);
				ack_arrival_count++;
				if(congestion_window<ssthresh)
				{
					next_congestion_window=next_congestion_window+1;
				}
				else
				{
					next_congestion_window=next_congestion_window+(1/next_congestion_window);
				}
				
			}
		}
		congestion_window=next_congestion_window;
			
	}
	char* fin=(char *)malloc(17*sizeof(char));
	fin="99999999\0";
	int finlen=strlen(fin)+1;
	sendto(s, fin/*dynamic_buffer*/, finlen, 0,(struct sockaddr *)&sin, send_len);
	/*storing congestion_window buffer into a file*/
	FILE *cwptr;
	FILE *tptr;
	FILE *ackptr;
	FILE *ackptrtime;
	FILE *lostptr;
	FILE *lostptrtime;
	FILE *timestampptr;
	if ((cwptr=fopen("cwnd.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<congestion_window_count;i++)
	{
		fprintf(cwptr,"%d\n",congestion_window_buffer[i]);
	}
	fclose(cwptr);
	if ((tptr=fopen("time.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<congestion_window_count;i++)
	{
		fprintf(tptr,"%d\n",i+1);
	}
	fclose(tptr);
	if ((ackptr=fopen("ack_arrival.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<ack_arrival_count;i++)
	{
		fprintf(ackptr,"%d\n",ack_arrival[i]);
	}
	fclose(ackptr);
	if ((ackptrtime=fopen("ack_arrival_time.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<ack_arrival_count;i++)
	{
		fprintf(ackptrtime,"%d\n",i+1);
	}
	fclose(ackptrtime);
	if ((lostptr=fopen("packet_lost.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<ack_lost_count;i++)
	{
		fprintf(lostptr,"%d\n",ack_lost[i]);
	}
	fclose(lostptr);
	if ((lostptrtime=fopen("packet_lost_time.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<ack_lost_count;i++)
	{
		fprintf(lostptrtime,"%d\n",i+1);
	}
	fclose(lostptrtime);
	if ((timestampptr=fopen("timestamp.txt"/*update_file*/,"w"))==NULL)
	{
	   	printf("Error! writing file");
	   	exit(1);
	}
	for(int i=0;i<timestamp_count;i++)
	{
		fprintf(timestampptr,"%d\n",timestamp[i]);
	}
	fclose(timestampptr);
}

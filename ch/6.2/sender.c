#include <stdlib.h>                                                                                                  
#include <fcntl.h>                                                                                                   
#include <stdio.h>                                                                                                   
#include <sys/types.h>                                                                                               
#include <sys/socket.h>                                                                                              
#include <netinet/in.h>                                                                                              
#include <string.h>                                                                                                  
#include <arpa/inet.h>                                                                                               
#include <unistd.h>                                                                                                  
#include <time.h>  
#define MAX_BUFFER_SIZE 600
#define MSS 100

int calculate_effective_window(int max_window, int last_byte_sent, int last_byte_ack){
	return max_window - (last_byte_sent – last_byte_ack);
}
int main(){
	int sockfd1,connfd1=0,sockfd2,connfd2=0;                                                                     
	struct sockaddr_in serv_addr;                                                                                
	char buffer[1024];                                                                                           
	char file[100],user[100],pass[100],new[10];                                                                  
	serv_addr.sin_family = AF_INET;                                                                              
	serv_addr.sin_port = htons(4444);                                                                            
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");                                                          
	sockfd1=socket(AF_INET, SOCK_STREAM, 0); 
	int n=2500;
	int ISN=0;
	int initial_sequence_number=ISN;
	int max_window = MAX_BUFFER_SIZE;
	int last_byte_sent = ISN;
	int last_byte_ack = ISN - 1;
	if(sockfd1<0){                                                                                               
		perror("socket creation failed");
		exit(EXIT_FAILURE);
		connfd1 = connect(sockfd1, (struct sockaddr*)&serv_addr, sizeof(serv_addr));                                 if(connfd1<0){                                                                                                  	printf("server accept failed\n");                                                                  
  		return 1;           
	}
	else
	printf("connection established\n");  
	while(1){
		int effective_window =calculate_effective_window( max_window, last_byte_sent, last_byte_ack);
		if (effective_window > o && n > 0){
			int data_start = initial_sequence_number;
			int data_end = initial_sequence_number +MSS -1;
			char packet[100];
			snprintf(packet, sizeof(packet), "SEQ=%d, DATA=%d", initial_sequence_number, data_start, data_end);
			sendto(sockfd,packet, strlen(packet) , 0,(struct sockaddr*)&server_add, sizeof(server_addr));
			last_byte_sent += MSS;
			n -=MSS;
			printf("S sends %s\n,packet");
		}



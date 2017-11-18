#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h> 
#include <math.h>

int main()
{
	///////////////////code sample for UDP server configuration from beejs guide starts 	
	int udpSocket;
	char buffer[1024];
	struct sockaddr_in serverCAddr;
	struct sockaddr_storage clientAddr;
	socklen_t addr_size, client_addr_size;
	int i,s_ans;
	double recv_data,answer;

	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
	serverCAddr.sin_family = AF_INET;
	serverCAddr.sin_port = htons(23159);
	serverCAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverCAddr.sin_zero, '\0', sizeof serverCAddr.sin_zero);  


	bind(udpSocket, (struct sockaddr *) &serverCAddr, sizeof(serverCAddr));
	addr_size = sizeof clientAddr;

	/////////////////// ends 
	printf("Server C is up and running using portno 23159\n");
	while(1)
	{
		
		bzero(buffer,1024);
		recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&clientAddr, &addr_size);///recvfrom socket function to receive data from AWS
		printf("The server C received input < %s > \n",buffer);
		recv_data = atof(buffer);///string to float conversion
		answer = recv_data*recv_data*recv_data*recv_data*recv_data;
		bzero(buffer,256);
		snprintf(buffer,1024,"%0.10f",answer);///float to string soncersion
		printf("The server C calculated 5th power < %s > \n",buffer);
		sendto(udpSocket,buffer,1024,0,(struct sockaddr *)&clientAddr,addr_size);///sendto socket function to send data to AWS
		printf("The server C finished sending the output to AWS\n");
	}

  return 0;
}


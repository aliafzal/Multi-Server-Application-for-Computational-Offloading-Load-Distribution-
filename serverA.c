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
	struct sockaddr_in serverAAddr;
	struct sockaddr_storage clientAddr;
	socklen_t addr_size, client_addr_size;
	int i,s_ans;
	double recv_data,answer;

	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
	serverAAddr.sin_family = AF_INET;
	serverAAddr.sin_port = htons(21159);
	serverAAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAAddr.sin_zero, '\0', sizeof serverAAddr.sin_zero);  
	bind(udpSocket, (struct sockaddr *) &serverAAddr, sizeof(serverAAddr));
	addr_size = sizeof clientAddr;
	/////////////////// ends 
	printf("Server A is up and running using portno 21159\n");
	while(1)
	{
		
		bzero(buffer,1024);
		recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&clientAddr, &addr_size);///recvfrom socket function to receive data from AWS
		printf("The server A received input < %s > \n",buffer);
		recv_data = atof(buffer);///string to float conversion
		answer = recv_data*recv_data;
		bzero(buffer,256);
		snprintf(buffer,1024,"%0.10f",answer); ///float to string soncersion 
		printf("The server A calculated square < %s > \n",buffer);
		sendto(udpSocket,buffer,1024,0,(struct sockaddr *)&clientAddr,addr_size);///sendto socket function to send data to AWS
		printf("The server A finished sending the output to AWS\n");
	}

  return 0;
}


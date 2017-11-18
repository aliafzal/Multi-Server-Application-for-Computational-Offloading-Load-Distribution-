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
	struct sockaddr_in serverBAddr;
	struct sockaddr_storage clientAddr;
	socklen_t addr_size, client_addr_size;
	int i,s_ans;
	double recv_data,answer;

	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);
	serverBAddr.sin_family = AF_INET;
	serverBAddr.sin_port = htons(22159);
	serverBAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverBAddr.sin_zero, '\0', sizeof serverBAddr.sin_zero);  


	bind(udpSocket, (struct sockaddr *) &serverBAddr, sizeof(serverBAddr));

	/////////////////// ends 
	addr_size = sizeof clientAddr;
	printf("Server B is up and running using portno 22159\n");
	while(1)
	{
		
		bzero(buffer,1024);
		recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&clientAddr, &addr_size);///recvfrom socket function to receive data from AWS
		printf("The server B received input < %s > \n",buffer);
		recv_data = atof(buffer);///string to float conversion
		answer = recv_data*recv_data*recv_data;
		bzero(buffer,256);
		snprintf(buffer,1024,"%0.10f",answer);///float to string soncersion
		printf("The server B calculated cube < %s > \n",buffer);
		sendto(udpSocket,buffer,1024,0,(struct sockaddr *)&clientAddr,addr_size);///sendto socket function to send data to AWS
		printf("The server B finished sending the output to AWS\n");
	}

  return 0;
}


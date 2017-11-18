

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h> //inet_addr

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

void str_filter(char* dest1, char* src )
{
	int a =0;
	for(a=0;a<strlen(src)-1;a++)
	{

		dest1[a] = src[a+1];
	}
	dest1[a+1] = '\0';
}
void rem_trail_zeros(char *val,int n)
{
	int b = 0;
	for( b = n; b>0 ; b--)
	{
		if(val[b-1] != '0')
		{
			val[b+1] = '\0';			
			break;
		}
	}


}
int main(int argc,char*argv[])
{
	int sock_aws,sock_com,sock_awsUDP,portno,portno2;
	socklen_t client ,bndservA_size,bndservB_size,bndservC_size;
	char buffer[1024];
	char res1[1024],res2[1024],res3[1024],res4[1024],res5[1024],fun[20];
	int dataSize,val_conv;
	char num[1024], function;
	struct sockaddr_in aws_addr,cli_addr,aws_UDP, bndservA,bndservB,bndservC;
	int n,s_ans;
	double recv_val,answer,input,recv_data[6];
	char ans[13];
	
	//creating TCP socket as described in beej's sample
	sock_aws = socket(AF_INET,SOCK_STREAM,0);
	bzero((char*) &aws_addr,sizeof(aws_addr));
	portno = 25159;
	aws_addr.sin_family = AF_INET;
	aws_addr.sin_port = htons(portno);
	if (bind(sock_aws,(struct sockaddr*)&aws_addr,sizeof(aws_addr))<0)
	{
		error("ERROR on binding");
	}

	if (sock_aws < 0)
	{
		error("ERROR opening TCP socket");
		
	}

	//creating UDP socket as described in beej's sample
	sock_awsUDP = socket(PF_INET, SOCK_DGRAM, 0);
	aws_UDP.sin_family = AF_INET;
	portno2 = 24159;
	aws_UDP.sin_port = htons(portno2);
	aws_UDP.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(aws_UDP.sin_zero, '\0', sizeof aws_UDP.sin_zero);  
  
	/*Bind socket with address struct*/
	bind(sock_awsUDP, (struct sockaddr *) &aws_UDP, sizeof(aws_UDP));

	//setting backend server address
	bndservA.sin_family = AF_INET;
	bndservA.sin_port = htons(21159);
	bndservA.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(bndservA.sin_zero, '\0', sizeof bndservA.sin_zero);
	bndservA_size = sizeof bndservA;

	bndservB.sin_family = AF_INET;
	bndservB.sin_port = htons(22159);
	bndservB.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(bndservA.sin_zero, '\0', sizeof bndservB.sin_zero);
	bndservB_size = sizeof bndservB;

	bndservC.sin_family = AF_INET;
	bndservC.sin_port = htons(23159);
	bndservC.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(bndservC.sin_zero, '\0', sizeof bndservC.sin_zero);
	bndservC_size = sizeof bndservC;
	
	printf("The AWS is up and running \n");
// recieveing data from client 
	
	while(1)
	{
		//waiting for client to connect 
		listen (sock_aws,5);
		client = sizeof(cli_addr);
		sock_com = accept(sock_aws,(struct sockaddr*)&cli_addr,&client);
		if(sock_com < 0)
		{
			error("ERROR on accept");
		}
		bzero(buffer,1024);
		n = read(sock_com,buffer,255);	//reading data from client 
		str_filter(num,buffer);
		function = buffer[0];
		val_conv = atof(num);	
		if(function == '2')
			strcpy(fun,"DIV");
		else if(function == '1')
			strcpy(fun,"LOG");
		
		printf("The AWS has recieved < %s >and function=<%s> from the client using TCP over port number <%d>\n",num,fun,portno);
		printf("The AWS has sent < %s > to Backend-Server A\n",num);
		printf("The AWS has sent < %s > to Backend-Server B\n",num);
		printf("The AWS has sent < %s > to Backend-Server C\n",num);
		
		//sending to backend server A
		
		strcpy(buffer,num);
		dataSize = strlen(buffer) + 1;
		sendto(sock_awsUDP,buffer,dataSize,0,(struct sockaddr *)&bndservA,bndservA_size);
		
		//reciving from server A
		bzero(buffer,1024);
		dataSize = recvfrom(sock_awsUDP,buffer,256,0,(struct sockaddr *)&bndservA,(socklen_t*)bndservA_size);
		strcpy(res1,buffer);
		recv_data[0] = atof(res1);
		//sending to backend server B
		strcpy(buffer,num);
		dataSize = strlen(buffer) + 1;
		sendto(sock_awsUDP,buffer,dataSize,0,(struct sockaddr *)&bndservB,bndservB_size);
		//reciving from server B
		bzero(buffer,1024);
		dataSize = recvfrom(sock_awsUDP,buffer,256,0,(struct sockaddr *)&bndservB,(socklen_t*)bndservB_size);
		strcpy(res2,buffer);
		recv_data[1] = atof(res2);


		//sending to backend server C
		strcpy(buffer,num);
		dataSize = strlen(buffer) + 1;
		sendto(sock_awsUDP,buffer,dataSize,0,(struct sockaddr *)&bndservC,bndservC_size);
		//reciving from server C
		bzero(buffer,1024);
		dataSize = recvfrom(sock_awsUDP,buffer,256,0,(struct sockaddr *)&bndservC,(socklen_t*)bndservC_size);
		strcpy(res3,buffer);
		recv_data[2] = atof(res3);
		printf("The AWS received < %s > Backend-Server <A> using UDP over port < %d > \n",res1,portno2);
		printf("The AWS received < %s > Backend-Server <B> using UDP over port < %d > \n",res2,portno2);
		printf("The AWS received < %s > Backend-Server <C> using UDP over port < %d > \n",res3,portno2);

		printf("The AWS has sent < %0.10f > to Backend-Server A\n",recv_data[0]);
		printf("The AWS has sent < %0.10f > to Backend-Server B\n",recv_data[0]);
		
		//sending to backend server A
		strcpy(buffer,res1);
		dataSize = strlen(buffer) + 1;
		sendto(sock_awsUDP,buffer,dataSize,0,(struct sockaddr *)&bndservA,bndservA_size);
		//reciving from server A
		bzero(buffer,1024);
		dataSize = recvfrom(sock_awsUDP,buffer,256,0,(struct sockaddr *)&bndservA,(socklen_t*)bndservA_size);
		strcpy(res4,buffer);
		recv_data[3] = atof(res4);
		
		//sending to backend server B
		strcpy(buffer,res1);
		dataSize = strlen(buffer) + 1;
		sendto(sock_awsUDP,buffer,dataSize,0,(struct sockaddr *)&bndservB,bndservB_size);
		//reciving from server B
		bzero(buffer,1024);
		dataSize = recvfrom(sock_awsUDP,buffer,256,0,(struct sockaddr *)&bndservB,(socklen_t*)bndservB_size);
		strcpy(res5,buffer);
		recv_data[4] = atof(res5);
		printf("The AWS received < %0.10f > Backend-Server <A> using UDP over port < %d > \n",recv_data[3],portno2);
		printf("The AWS received < %0.10f > Backend-Server <B> using UDP over port < %d > \n",recv_data[4],portno2);
		printf("Values of power received by AWS: < %s, %s, %s, %s, %s, %s >\n",num,res1,res2,res3,res4,res5);

		
		
		if(function == '2')
		{
			answer = 1 + atof(num) + recv_data[0]+ recv_data[1]+ recv_data[2]+ recv_data[3]+ recv_data[4];
			printf("AWS calculated DIV on <%s> : <%0.10f>\n",num,answer);
			bzero(buffer,256);
			snprintf(buffer,1024,"%0.10f",answer);
			n = write(sock_com,buffer,strlen(buffer)); //sending data back to client 
			printf("AWS sent <%s> to client\n",buffer);
			if(n < 0)
			{
				error("ERROR writing to socket");
			}
		}
		else if(function == '1')
		{
			answer = (-atof(num) - recv_data[0]/2 - recv_data[1]/3 - recv_data[2]/4 - recv_data[3]/5 - recv_data[4]/6) ;
			printf("AWS calculated LOG on <%s> : <%f>\n",num,answer);
			bzero(buffer,256);
			snprintf(buffer,1024,"%0.10f",answer);
			n = write(sock_com,buffer,strlen(buffer));//sending data back to client 
			printf("AWS sent <%s> to client\n",buffer);
			if(n < 0)
			{
				error("ERROR writing to socket");
			}	
		}

		close(sock_com);
		
	}
	close(sock_aws);
	return 0;

}

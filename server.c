//////////////////////Ali Afzal///////////////
/////////////////////E-12-008/////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc,char*argv[])
{
	int sockfd,newsockfd,portno;
	socklen_t client;
	char buffer[256];
	char pl,mi,mx,di;
	int pl1,mi1,mx1,di1;
	pl = '+';
	mi = '-';
	mx = '*';
	di = '/';
	//pl1 = atoi(pl);
	//mi1 = atoi(mi);
	//mx1 = atoi(mx);
	//di1 = atoi(di);
	struct sockaddr_in serv_addr,cli_addr;
	int n2,n1,n,s,oper;
	char ans[13];
	if (argc < 2)
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
		
	}
	bzero((char*) &serv_addr,sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
	{
		error("ERROR on binding");
	}
	listen (sockfd,5);
	client = sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr,&client);
	if(newsockfd < 0)
	{
		error("ERROR on accept");
	}
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	printf ("First Number = %s",buffer);
	n1=atoi(buffer);
	if (n < 0)
	{
		error("ERROR reading from socket");
	}
	n = read(newsockfd,buffer,255);
	printf ("Second Number = %s",buffer);
	n2=atoi(buffer);	
	if (n < 0)
	{
		error("ERROR reading from socket");
	}
	n = read(newsockfd,buffer,255);
	printf ("Operator is : %s",buffer);	
	if (n < 0)
	{
		error("ERROR reading from socket");
	}
	oper = atoi(buffer);
	if (strcmp(buffer,"+")>0)
	{
		s = n2+n1;
		printf("Oper + ");
	}
	else if (strcmp(buffer,"-")>0) 
	{
		s = n1-n2;
		printf("Oper - ");	
	}
	else if (strcmp(buffer,"*")>0) 
	{
		s = n1*n2;
		printf("Oper * ");	
	}
	else if (strcmp(buffer,"/")>0) 
	{
		s = n1/n2;
	}
	else
	{
		printf("Unknown Operator");
	}
	printf("The answer is : %d",s);
	printf("\n");
	sprintf(buffer,"%d",s);
	n = write(newsockfd,buffer,strlen(buffer));
	
	if(n < 0)
	{
		error("ERROR writing to socket");
	}
	close(newsockfd);
	close(sockfd);
	return 0;

}

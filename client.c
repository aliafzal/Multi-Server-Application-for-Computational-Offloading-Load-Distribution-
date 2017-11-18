#include<stdio.h> 
#include<string.h>   
#include<sys/socket.h>   
#include<arpa/inet.h>
#include<unistd.h>
 
int main(int argc , char *argv[])
{

	
	int sock,ch;
	struct sockaddr_in aws_servaddr;
	char function[5],val[15],s_data[20],aws_reply[256];
	int inp_val;
//////////// setting server socket address take from TCP code sample for beej's guide starts     
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	
	aws_servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	aws_servaddr.sin_family = AF_INET;
	aws_servaddr.sin_port = htons( 25159 );

//////////////////////////ends
	if (connect(sock , (struct sockaddr *)&aws_servaddr , sizeof(aws_servaddr)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	printf("The client is up and running\n");
	strcpy(function,argv[1]);
	strcpy(val,argv[2]);
//////////////////////////sending right data to aws server 
	if(strcmp(function,"DIV"))
	{
		strcpy(function,"1");
		strcpy(s_data,function);
		strcat(s_data,val);
		ch = write(sock,s_data,strlen(s_data));
		printf("The client sent < %s > and %s to AWS \n",val,argv[1]);
		if(ch<0)
		puts("Sending failed");
	}
	else if(strcmp(function,"LOG"))
	{
		strcpy(function,"2");
		strcpy(s_data,function);
		strcat(s_data,val);
		ch = write(sock,s_data,strlen(s_data));
		printf("The client sent < %s > and %s to AWS \n",val,argv[1]);
		if(ch<0)
		puts("Sending failed");
	}
//receving data from aws server
	bzero(aws_reply,255);
	if(read(sock,aws_reply,255) < 0)
	{
	    puts("recv failed");
	}
	printf("According to AWS, %s on < %s >:< %s > \n",argv[1],val,aws_reply);	
	close (sock);
	return 0;
}

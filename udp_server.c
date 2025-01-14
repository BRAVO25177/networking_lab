#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
int main()
{
	struct sockaddr_in server_addr, client_addr;
	int socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	char server_msg[2000], client_msg[2000];
	
	memset(server_msg, '\0', sizeof(server_msg));
	memset(client_msg, '\0', sizeof(client_msg));
	
	if(socket_desc<0)
	{
		printf("Socket not created\n");
		return -1;
	}
	printf("Socket created\n");
	server_addr.sin_port = htons(2000);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
	{
		printf("Bind unsuccessful\n");
		return -1;
	}
	printf("Bind successful\n");
	
	socklen_t client_len = sizeof(client_addr);
	
	if(recvfrom(socket_desc, client_msg, sizeof(client_msg), 0, (struct sockaddr*)&client_addr, &client_len)<0)
	{
		printf("Error\n");
		return -1;
	}
	printf("Message from client: %s\n", client_msg);
	printf("Enter message:");
	scanf("%s", server_msg);
	if(sendto(socket_desc, server_msg, sizeof(server_msg), 0, (struct sockaddr*)&client_addr, client_len)<0)
	{
		printf("Error\n");
		return -1;
	}
	
	close(socket_desc);
	return 0;
}


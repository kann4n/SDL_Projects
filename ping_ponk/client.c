#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
	struct sockaddr_in serv_addr;
	const char *serv_ip = "127.0.0.1";
	const unsigned short serv_port = 2005; 
	char buffer[256];

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("Error: socket creation failed");
		exit(EXIT_FAILURE);
	}

	// setup address
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(serv_port);
	if(inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr) <= 0)
	{
		fprintf(stderr, "Error: invalid address\n");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// connect to server
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Error: connection failed");
		close(sockfd);
		return -3;
	}

	printf("Connected to server on port %d\n", serv_port);

	memset(buffer, 0, sizeof(buffer));
	int valread = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

	if (valread > 0) 
	{
		printf("Server: %s\n", buffer);
	} 
	else if (valread == 0) {
		printf("Server closed the connection.\n");
	} else {
		perror("recv failed");
	}

	printf("Closing connection.\n");
	close(sockfd);
	return 0;
}

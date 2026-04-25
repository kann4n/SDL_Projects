#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main() 
{
	// server and new socket file discriptor
	int serverfd, clientfd;
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	char *buffer = "hello its working";
	const char *serv_ip = "127.0.0.1";
	const unsigned short serv_port = 2005;

	// setup socketFD
	if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // domain IPv4, type TCP
	{
		perror("Error: socket setup failed");
		exit(EXIT_FAILURE);
	}

	// setup addr
	addr.sin_family = AF_INET; 		// family
	addr.sin_port = htons(serv_port); 	// to match endianness
						// convert percention_f to internet_n.
	if(inet_pton(AF_INET, serv_ip, &addr.sin_addr) <= 0)
	{
		perror("Error: setup server ip address");
		close(serverfd);
		exit(EXIT_FAILURE);
	}

	// bind 
	if (bind(serverfd, (struct sockaddr *)&addr, addrlen) < 0)
	{
		perror("Error: bind failed");
		close(serverfd);
		exit(EXIT_FAILURE);
	}
	// listen
	if (listen(serverfd, 4) == -1)
	{
		perror("Error: listen failled");
		close(serverfd);
		exit(EXIT_FAILURE);
	}

	printf("server listening on port %d\n", serv_port);

	// accept new connections
	clientfd = accept(serverfd, (struct sockaddr *)&addr, (socklen_t*)&addrlen); 
	if (clientfd < 0)
	{
		perror("Error: accept failed");
	}
	// test server
	send(clientfd, buffer, strlen(buffer), 0);

	close(clientfd);
	close(serverfd);
	return 0;
}

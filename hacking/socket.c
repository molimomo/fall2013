#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <limits.h>

#define MAXDATA 4096 

int main(int argc, char *argv[]) {
	int sockfd; // socket handle
	char *hostname;
	long int port;
	struct sockaddr_in serveraddr;
	struct hostent *server;	
	int numbytes;  // number of bytes read or written
	char buf[MAXDATA]; // buffer to hold the data read
	unsigned int i; // iterator
	uint32_t result; // single unsigned int 
	uint32_t sum = 0; // sum of read unsigned ints

	//Check command line arguments
	if (argc != 3) {
		fprintf(stderr,"usage: hostname port\n");
		exit(1);
	}
	
	//Check port
	hostname = argv[1];
	port = strtol(argv[2], (char **)NULL, 10);
	if (port == LONG_MIN || port == LONG_MAX || port == 0) {
		perror("can't parse port");
		exit(2);
	}
	
	//Create socket, get hostname
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("can't create socket");
		exit(1);
	}
	server = gethostbyname(hostname);
	if (server == NULL) {
		perror("can't lookup host");
		exit(1);
	}
	
	//Create Serveraddr struct
	bzero((char *) &serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(port);	

	if (connect(sockfd, (const struct sockaddr *)&serveraddr,
sizeof(serveraddr)) < 0) {
		perror("can't connect");
		exit(1);
	}

	printf("client: connecting to %s:%s\n", argv[1], argv[2]);
	for(i=0;i<=3;i++) {
		recv(sockfd, &result, 4, 0);
		sum += result;
	}

	write(sockfd,&sum, 4);
	if ((numbytes = recv(sockfd, buf, MAXDATA-1, 0)) == -1) {
		perror("receive error");
		exit(1);
	}
	buf[numbytes] = '\0';
	printf("client: received %s",buf);
	printf("\n");
	close(sockfd);
	
	return 0;
}

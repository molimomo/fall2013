#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <dirent.h>
/* You will have to modify the program below */

#define MAXBUFSIZE 256
#define MAXMSGSIZE 50000

int main (int argc, char * argv[] )
{
	int sockfd;                           //This will be our socket
	int toSend;
	struct sockaddr_in sin, remote;     //"Internet socket address structure"
	socklen_t remote_length;         //length of the sockaddr_in structure
	int nbytesR, nbytesS, nbytesS2, nbytesR2;                        //number of bytes we receive in our message
	char buffer[MAXMSGSIZE];             //a buffer to store our received message
	char msg[MAXMSGSIZE];				//response to send back to client.
	int done = 0;
	if (argc != 2)
	{
		printf ("USAGE:  <port>\n");
		exit(1);
	}

	/******************
	  This code populates the sockaddr_in struct with
	  the information about our socket
	 ******************/
	bzero(&sin,sizeof(sin));                   //zero the struct
	sin.sin_family = AF_INET;                   //address family
	sin.sin_port = htons(atoi(argv[1]));        //htons() sets the port # to network byte order
	sin.sin_addr.s_addr = INADDR_ANY;           //supplies the IP address of the local machine


	//Causes the system to create a generic socket of type UDP (datagram)
	sockfd = socket(PF_INET,SOCK_DGRAM,0);
	if (sockfd < 0)
	{
		printf("unable to create socket");
		close(sockfd);
		return 0;
	}

	/******************
	  Once we've created a socket, we must bind that socket to the 
	  local address and port we've supplied in the sockaddr_in struct
	 ******************/
	if (bind(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		printf("unable to bind socket\n");
		exit(0);
	}
	
	
	while(!done){
		//waits for an incoming message
		remote_length = sizeof remote;
		bzero(buffer,sizeof(buffer));
		nbytesR = recvfrom(sockfd, buffer, MAXMSGSIZE, 0, (struct sockaddr*)&remote, &remote_length);
		if(nbytesR < 0){
			printf("Server Says: Error Receiving from client\n");
		}

		printf("The client says %s\n", buffer);
		char file[MAXBUFSIZE];
		char command[MAXBUFSIZE];
		int n = sscanf(buffer, "%s %s", command, file);
		
		//ls command
		if(strcmp(command, "ls") == 0 && n == 1){
			bzero(msg,sizeof(msg));
		
			DIR *dp;
			struct dirent *ep;     
			dp = opendir ("./");

			if (dp != NULL){
				while (ep = readdir (dp)){
					strcat(msg, strcat(ep->d_name, "\n"));
				}
				(void) closedir (dp);
			}
			else
				perror ("Couldn't open the directory");
				
			toSend = strlen(msg);	
		//exit command		
		}else if(strcmp(command, "exit") == 0 && n==1){
			bzero(msg,sizeof(msg));
			strcpy(msg, "Server closing, cya!");
			toSend = strlen(msg);
			done = 1;
			printf("Server shutting down, cya!\n");
		
		//get Command	
		}else if(n == 2 && strcmp(command, "get") == 0){
			bzero(msg,sizeof(msg));
			FILE *fp; 
			long file_size;
			
			//open file
			fp=fopen(file, "rb"); 	
			if(fp == NULL){
				printf("error opening file on server\n");
				strcpy(msg, "Error opening file on server!");
				toSend = strlen(msg);
			}else{
				
				//obtain file size
				fseek(fp, 0, SEEK_END);
				file_size = ftell(fp);
				rewind(fp);
			
				//read from file
				size_t result = fread(msg, 1, file_size, fp);
				if(result != file_size){
					printf("File reading error!");
					fclose(fp);
				}
				toSend = result;
				fclose(fp);
			}
				
		//Put command	
		}else{	
			
			//Got file in buffer now request filename
			bzero(msg,sizeof(msg));
			strcpy(msg, "FilenameRequest");
			toSend = strlen(msg);
			nbytesS2 = sendto(sockfd, msg, strlen(msg), 0,  (struct sockaddr *)&remote, sizeof(remote));
			if(nbytesS2 < 0){
				printf("Server Says: Error Sending filename request to client\n");
			}
			
			//Wait for filename
			bzero(&file,sizeof(file)); 
			nbytesR2 = recvfrom(sockfd, file, MAXBUFSIZE, 0, (struct sockaddr*)&remote, &remote_length);
			if(nbytesR2 < 0){
				printf("Server Says: Error Sending filename request to client\n");
			}

			//write file
			FILE * fp;
			fp = fopen(file, "wb");
			if(fwrite(buffer, 1, nbytesR, fp) != nbytesR || fp == NULL){
				bzero(msg,sizeof(msg));
				strcpy(msg, "File reception failure!\n");
				toSend = strlen(msg);
			}else{
				bzero(msg,sizeof(msg));
				strcpy(msg, "File reception success!\n");
				toSend = strlen(msg);
			}
			fclose(fp);	
		}

		nbytesS = sendto(sockfd, msg, toSend, 0,  (struct sockaddr *)&remote, sizeof(remote));
		if(nbytesS < 0){
			printf("Server says: Error sending to client\n");
		}
	}
	
	if(shutdown(sockfd, 0) < 0){
		printf("error closing socket\n");
		exit(-1);
	}
	
	return 0;
}


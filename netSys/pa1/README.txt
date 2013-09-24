Programming Assignment 1 - UDP FTP Server and Client
Gabe Russotto - Gabriele.Russotto@colorado.edu
Fall 2013
Network Systems

This program is a simple FTP client capable of transfering files of no 
more than 5KB in size both to and from the working directory that the client
and server are running in. It is implemented in C using basic socket protocols.

Compliation: 
Simply run make, in the directory with both the make file and udp_server.c
and udp_client.c and two exectuables server and client will be compiled.

Usage:
	Server: To run the server run the command enter $./server <port_number>
		on the command line of the correct working directory. Where
		port number is the port in which the server will be listening for
		the client.
	
	Client: To run the client enter $./client <server_ip> <port_number>
		on the command line of the correct working directory. Where
		port number and server ip are the IP and port number in which the
		server program is listening.
	
	Client Commands:
		“get <file_name>”: The server transmits the requested file to the
		client. If the filename already exists it is overwritten.

		“put <file_name>”: The server receives the transmitted file by 
		the client and stores it locally. If the filename already exists
		it is overwritten.

		“ls”: The server should search all the files it has in its local 
		directory and send a list of all these files to the client.

		“exit” : The server and client should both exit gracefully.
		
		
		
		
	


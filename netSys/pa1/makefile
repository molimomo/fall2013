all: client server
client: udp_client.c
	gcc -o client udp_client.c -I.
server: udp_server.c
	gcc -o server udp_server.c -I.
clean:
	rm -f client server *.o




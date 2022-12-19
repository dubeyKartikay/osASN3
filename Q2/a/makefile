all: client server
client: client.c
	gcc -o client -O3  client.c
server: server.c
	gcc -o server -O3  server.c
clean:
	rm -f client server socket

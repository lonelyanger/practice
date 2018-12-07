PROGS:server client 

all:	${PROGS}

server:
	cc server.c -o server -lunp

client:
	cc client.c -o client -lunp


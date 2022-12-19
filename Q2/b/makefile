all: p1 p2

p1: P1.c
	gcc -o p1 P1.c -O3

p2: P2.c
	gcc -o p2 P2.c -O3

run: p1 p2
	./p1 &
	./p2

clean:
	rm -f p1 p2 fifo1 fifo2

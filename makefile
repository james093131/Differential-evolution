all:
	g++ -Wall -O3 -o DE main.cpp

clean:
	rm -f main *.o

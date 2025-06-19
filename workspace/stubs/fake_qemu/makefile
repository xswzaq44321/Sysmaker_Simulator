main: main.o socket_api.o
	g++ $^ -o $@

main.o: main.cpp socket_api.h
	g++ -c main.cpp -o main.o
socket_api.o: socket_api.c socket_api.h
	g++ -c socket_api.c -o socket_api.o

clean:
	rm *.o
	rm main
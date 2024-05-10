Client: Client.cpp socket.o Blockable.o
	g++ -o Client Client.cpp socket.o Blockable.o -lcrypto -std=c++11

Blockable.o : Blockable.h Blockable.cpp
	g++ -c Blockable.cpp -std=c++11

socket.o : socket.cpp socket.h
	g++ -c socket.cpp -std=c++11


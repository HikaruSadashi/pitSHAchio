ComputeServer: ComputeServer.cpp socket.o socketserver.o Blockable.o IPCServer.o
	g++ -o ComputeServer ComputeServer.cpp IPCServer.o socket.o Blockable.o socketserver.o -std=c++11

IPCServer.o: Blockable.h socket.h IPCServer.cpp IPCServer.h
	g++ -c IPCServer.cpp socket.h Blockable.h socketserver.h -std=c++11

IPCsocket.o: IPCsocket.h IPCsocket.cpp
	g++ -c IPCsocket.cpp -std=c++11

Blockable.o : Blockable.h Blockable.cpp
	g++ -c Blockable.cpp -std=c++11

thread.o : thread.cpp thread.h
	g++ -c thread.cpp -std=c++11

socket.o : socket.cpp socket.h
	g++ -c socket.cpp -std=c++11

socketserver.o : socketserver.cpp socket.h socketserver.h
	g++ -c socketserver.cpp -std=c++11


#include "Network.h"
#include <string>
using std::string;

// Connects to the server
TCPsocket ConnectToServer(const string& ipaddress){
	IPaddress ip;
	if(SDLNet_ResolveHost(&ip, ipaddress.c_str(), 9999) == -1) {
		printf("Resolve: %s\n", SDLNet_GetError());
		return 0;
	}
	return SDLNet_TCP_Open(&ip);
}

// Sends data with size to the socket
bool Send(const void* data, int size, TCPsocket socket){
	if (!socket){
		return false;
	}
	int result1 = SDLNet_TCP_Send(socket, data, size);
	if(result1 < size) {
		printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		return false;
	}
	return true;
}

// Accepts incoming connections
bool Recieve(void *data, int size, TCPsocket socket){
	if (!socket){
		return false;
	}
	int result = SDLNet_TCP_Recv(socket, data, size);
	if(result <= 0) {
		return false;
	}
	return true;
}

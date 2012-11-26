#include "Network.h"

#include <string>
using std::string;

// Listen for new incoming sockets. Use this when you want new clients to connect to the server
TCPsocket ListenForClient(TCPsocket server){
	return SDLNet_TCP_Accept(server);
}

// Creates the server socket. Pass this socket to the ListenForClient function.
TCPsocket CreateServer()
{
	// create a listening TCP socket on port 9999 (server)
	IPaddress ip;

	if(SDLNet_ResolveHost(&ip, NULL, 9999) == -1) {
	    printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
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
		// It may be good to disconnect sock because it is likely invalid now.
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

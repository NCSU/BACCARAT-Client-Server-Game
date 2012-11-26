#include <SDL/SDL_net.h>
#include <string>
using std::string;

// Listen for new incoming sockets. Use this when you want new clients to connect to the server.
TCPsocket ListenForClient(TCPsocket server);

// Creates the server socket. Pass this socket to the ListenForClient function.
TCPsocket CreateServer();

// Sends data with size to the socket. This functions does not block the program.
bool Send(const void* data, int size, TCPsocket socket);

// Receives data on the socket. Blocks the program until the data is received.
bool Recieve(void* data, int size, TCPsocket socket);

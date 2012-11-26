#include <SDL/SDL_net.h>
#include <string>
using std::string;

// Connects to the server. Use this to acquire socket for server interaction.
TCPsocket ConnectToServer(const string& ip);

// Send data with size to the socket. This functions does not block the program.
bool Send(const void* data, int size, TCPsocket socket);

// Receives data on the socket. Blocks the program until the data is received.
bool Recieve(void *data, int size, TCPsocket socket);

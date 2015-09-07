#include "network_utils.h"
#include <WinSock2.h>

bool TCP_createHostSocket(int maxConnections, unsigned short port, SOCKET *outSocket, SOCKADDR_IN *outAddress, networkStatus_t *status);
bool TCP_createSocket(const char *address, unsigned short port, SOCKET *outSocket, SOCKADDR_IN *outAddress, networkStatus_t *status);

bool TCP_connect(SOCKET socket, const SOCKADDR *address, networkStatus_t *status);
bool TCP_acceptNewClient(SOCKET hostSocket, SOCKET *socket, SOCKADDR *addr, int *addrLen);

bool TCP_sendMessage(SOCKET socket, bytestream message);
bool TCP_receiveMessages(SOCKET socket, bytestream *out);

#include "network_utils.h"
#include <WinSock2.h>

bool UDP_createSocket(const char *address, unsigned short port, SOCKET *outSocket, SOCKADDR_IN *outAddress, networkStatus_t *status);

bool UDP_sendMessage(bytestream message, SOCKET socket, const SOCKADDR *address, int addrLen);
bool UDP_receiveMessages(bytestream *out, SOCKET socket, SOCKADDR *address, int *addrLen);

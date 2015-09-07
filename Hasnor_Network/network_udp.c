#include "network_udp.h"

#pragma comment (lib, "ws2_32.lib")


bool UDP_createSocket(const char *address, unsigned short port, SOCKET *outSocket, SOCKADDR_IN *outAddress, networkStatus_t *status)
{
	SOCKET newSocket;

	outAddress->sin_addr.S_un.S_addr = address ? inet_addr(address) : ADDR_ANY;
	outAddress->sin_port = htons(port);
	outAddress->sin_family = AF_INET;

	newSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	*outSocket = newSocket;

	if (bind(newSocket, (const SOCKADDR*)outAddress, sizeof(*outAddress)) < 0)
	{
		if (status)
		{
			status->error = NETWORK_ERROR_BINDING;
			status->socketError = WSAGetLastError();
		}
		return false;
	}

	return true;
}

bool UDP_sendMessage(bytestream message, SOCKET socket, const SOCKADDR *address, int addrLen)
{
	return (sendto(socket, message.data, message.len, 0, address, addrLen) != SOCKET_ERROR);
}

bool UDP_receiveMessages(bytestream *out, SOCKET socket, SOCKADDR *address, int *addrLen)
{
	static char buffer[65536];
	
	int received = recvfrom(socket, buffer, 65536, 0, address, addrLen);
	if (received > 0)
	{
		bytestream_init(out, received);
		bytestream_write(out, buffer, received);
		out->cursor = 0;

		return true;
	}

	return false;
}
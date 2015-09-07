#include "network_tcp.h"

#pragma comment (lib, "ws2_32.lib")


bool TCP_createHostSocket(int maxConnections, unsigned short port, SOCKET *outSocket, SOCKADDR_IN *outAddress, networkStatus_t *status)
{
	SOCKET newSocket;

	outAddress->sin_addr.S_un.S_addr = ADDR_ANY;
	outAddress->sin_port = htons(port);
	outAddress->sin_family = AF_INET;

	newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	if (maxConnections < 0)
	{
		maxConnections = SOMAXCONN;
	}

	if (listen(newSocket, maxConnections) == SOCKET_ERROR)
	{
		if (status)
		{
			status->error = NETWORK_ERROR_LISTEN;
			status->socketError = WSAGetLastError();
		}
		return false;
	}

	return true;
}

bool TCP_createSocket(const char *address, unsigned short port, SOCKET *outSocket, SOCKADDR_IN *outAddress, networkStatus_t *status)
{
	SOCKET newSocket;

	outAddress->sin_addr.S_un.S_addr = address ? inet_addr(address) : ADDR_ANY;
	outAddress->sin_port = htons(port);
	outAddress->sin_family = AF_INET;

	newSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	*outSocket = newSocket;

	return true;
}

int _checkSocketWritable(SOCKET socket)
{
	int temp;
	fd_set set;
	TIMEVAL timeout;

	memset(&set, 0, sizeof(fd_set));

	FD_SET(socket, &set);

	timeout.tv_sec = 0;
	timeout.tv_usec = 1000000;

	temp = select(socket+1, NULL, &set, NULL, &timeout);
	
	if (temp > 0)
	{
		return FD_ISSET(socket, &set);
	}
	
	return temp;
}

bool TCP_connect(SOCKET socket, const SOCKADDR *address, networkStatus_t *status)
{
	if (connect(socket, address, sizeof(*address)) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			if (!_checkSocketWritable(socket))
			{
				status->error = NETWORK_ERROR_CONNECT;
				status->socketError = 0;
				return false;
			}
		}
		else
		{
			status->error = NETWORK_ERROR_CONNECT;
			status->socketError = error;
			return false;
		}
	}

	return true;
}

bool TCP_acceptNewClient(SOCKET hostSocket, SOCKET *socket, SOCKADDR *addr, int *addrLen)
{
	SOCKET newSocket = accept(hostSocket, addr, addrLen);
	
	if (newSocket == INVALID_SOCKET || newSocket == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool TCP_sendMessage(SOCKET socket, bytestream message)
{
	return (send(socket, message.data, message.len, 0) != SOCKET_ERROR);
}

bool TCP_receiveMessages(SOCKET socket, bytestream *out)
{
	static char buffer[65536];
	
	int received = recv(socket, buffer, 65536, 0);
	if (received > 0)
	{
		bytestream_init(out, received);
		bytestream_write(out, buffer, received);
		out->cursor = 0;

		return true;
	}

	return false;
}
#include "network_internal.h"
#include <WinSock2.h>

#include "network_tcp.h"
#include "network_udp.h"

#pragma comment (lib, "ws2_32.lib")

typedef struct {
	int					id;

	SOCKET				socket;
	socketType_t		type;
	socketProtocol_t	protocol;
	SOCKADDR_IN			address;

	timestamp_ms_t		lastInActivity;		// Time at which we last received a message from this client
	timestamp_ms_t		lastOutActivity;	// Time at which we last sent a message to this client
} networkConnection_t;

networkMode_t _networkMode = NETWORK_MODE_LOCAL;
networkConnection_t *_connections = NULL;
uint _maxConnections = 0;

timestamp_ms_t _worry = 10000;		// Milliseconds before sending a heartbeat
timestamp_ms_t _timeout = 30000;	// Milliseconds before dropping a connection

networkMode_t currentNetworkMode(void)
{
	return _networkMode;
}

uint maxConnections(void)
{
	return _maxConnections;
}

void setupNetwork(networkMode_t mode)
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
}

void setTimeout(timestamp_ms_t worryTime, timestamp_ms_t timeoutTime)
{
	_worry = worryTime;
	_timeout = timeoutTime;
}

void shutdownNetwork(void)
{
	WSACleanup();
}

void _setupSocket(SOCKET socket)
{
	unsigned long mode = 1;
	ioctlsocket(socket, FIONBIO, &mode);
}

void _cleanupSocket(SOCKET *socket)
{
	closesocket(*socket);
	*socket = INVALID_SOCKET;
}

int _checkSocketReadable(SOCKET socket)
{ // Unused
	int temp;
	fd_set set;
	TIMEVAL timeout;

	Memory.set(&set, 0, sizeof(fd_set));

	FD_SET(socket, &set);

	timeout.tv_sec = 0;
	timeout.tv_usec = 1000000;

	temp = select(socket+1, &set, NULL, NULL, &timeout);
	
	if (temp > 0)
	{
		return FD_ISSET(socket, &set);
	}
	
	return temp;
}

bool _addNewSocket(SOCKET socket, socketType_t type, socketProtocol_t protocol, SOCKADDR_IN address)
{
	uint i = 0;
	while (i < _maxConnections)
	{
		if (_connections[i].socket == INVALID_SOCKET)
		{
			_connections[i].socket = socket;
			_connections[i].type = type;
			_connections[i].protocol = protocol;
			_connections[i].address = address;
			_connections[i].lastInActivity = _connections[i].lastOutActivity = Time.milliseconds();
			return true;
		}
		i++;
	}
	return false;
}

networkConnection_t *_connectionWithID(int id)
{
	uint i = 0;
	while (i < _maxConnections)
	{
		if (_connections[i].id == id)
		{
			return &_connections[i];
		}
		i++;
	}
	return NULL;
}

SOCKET _socketWithID(int id)
{
	networkConnection_t *connection = _connectionWithID(id);
	
	if (connection)
	{
		return connection->socket;
	}
	
	return INVALID_SOCKET;
}

void _setupMaxConnections(uint maxConnections)
{
	uint i;
	_maxConnections = maxConnections;
	_connections = allocArray(networkConnection_t, _maxConnections);
	Memory.set(_connections, 0, sizeof(networkConnection_t) * _maxConnections);
	for (i = 0; i < _maxConnections; i++)
	{
		_connections[i].id = i;
		_connections[i].socket = INVALID_SOCKET;
		_connections[i].type = SOCKET_TYPE_INACTIVE;
	}
}

bool createHostSocket(int maxConnections, unsigned short port, socketProtocol_t protocol, networkStatus_t *status)
{
	SOCKET newSocket;
	SOCKADDR_IN addr;

	if (protocol == SOCKET_PROTOCOL_TCP)
	{
		if (!TCP_createHostSocket(maxConnections, port, &newSocket, &addr, status))
		{
			_cleanupSocket(&newSocket);
			return false;
		}
	}
	else if (protocol == SOCKET_PROTOCOL_UDP)
	{
		if (!UDP_createSocket(NULL, port, &newSocket, &addr, status))
		{
			_cleanupSocket(&newSocket);
			return false;
		}
	}

	_setupMaxConnections(++maxConnections); // We count the first connection as the server's local socket

	_setupSocket(newSocket);
	_addNewSocket(newSocket, SOCKET_TYPE_HOST, protocol, addr);
	_connections[0].id = -1; // Guess this is needed for now
	
	_networkMode = NETWORK_MODE_HOST;

	return true;
}

bool createSocket(const char *address, unsigned short port, socketProtocol_t protocol, networkStatus_t *status)
{
	SOCKET newSocket;
	SOCKADDR_IN addr;

	if (protocol == SOCKET_PROTOCOL_TCP)
	{
		if (!TCP_createSocket(address, port, &newSocket, &addr, status))
		{
			_cleanupSocket(&newSocket);
			return false;
		}
	}
	else if (protocol == SOCKET_PROTOCOL_UDP)
	{
		if (!UDP_createSocket(address, port, &newSocket, &addr, status))
		{
			_cleanupSocket(&newSocket);
			return false;
		}
	}

	_setupMaxConnections(1);

	_setupSocket(newSocket);
	_addNewSocket(newSocket, SOCKET_TYPE_CLIENT, protocol, addr);

	return true;
}

bool tryToConnect(bytestream_t clientInfo, networkStatus_t *status)
{
	if (_connections[0].protocol == SOCKET_PROTOCOL_TCP)
	{
		if (!TCP_connect(_connections[0].socket, ((SOCKADDR*)&_connections[0].address), status))
		{
			return false;
		}
	}
	else
	{ // There is no proper connection in UDP, just say hello
		
	}

	_networkMode = NETWORK_MODE_CLIENT;

	sendMessage(NETWORK_MESSAGE_CONNECT, -1, -1, clientInfo);

	return true;
}

bool getNewClient(void)
{
	SOCKADDR inAddr;
	int addrLen = sizeof(inAddr);
	SOCKET newSocket = accept(_connections[0].socket, &inAddr, &addrLen);
	
	if (newSocket == INVALID_SOCKET || newSocket == SOCKET_ERROR)
	{
		return false;
	}

	if (_addNewSocket(newSocket, _connections[0].type, _connections[0].protocol, *((SOCKADDR_IN*)&inAddr)))
	{
		return true;
	}

	printf("No more open client slots!\n");
	return false;
}

void _closeConnection(networkConnection_t *connection, bool broadcast)
{
	bytestream_t temp;
	ByteStream.init(&temp, 0);
	sendMessage(NETWORK_MESSAGE_EXIT, -1, broadcast ? -1 : connection->id, temp);

	connection->id = 0;
	connection->type = SOCKET_TYPE_INACTIVE;

	// FIXME: Closing the socket here means this client won't always receive his exit message if the socket is already busy
	_cleanupSocket(&connection->socket);
}

void dropClient(int id)
{
	_closeConnection(_connectionWithID(id), true);
}

void disconnect(void)
{ // Close all active connections
	uint i;
	bytestream_t temp;
	ByteStream.init(&temp, 0);

	for (i = 0; i < _maxConnections; i++)
	{
		networkConnection_t *connection = &_connections[i];
		if (connection->socket != INVALID_SOCKET)
		{
			_closeConnection(connection, (_networkMode == NETWORK_MODE_CLIENT));
		}
	}

	_networkMode = NETWORK_MODE_LOCAL;
	_maxConnections = 0;
}

void checkForTimeOuts(void)
{
	long curTime = Time.milliseconds();

	if (_networkMode == NETWORK_MODE_CLIENT)
	{
		networkConnection_t *connection = &_connections[0];
		
		if ((curTime - connection->lastInActivity) > _timeout)
		{ // Lost connection to the server
			printf("Server timed out, dropping\n");
			disconnect();
		}
		else if ((curTime - connection->lastOutActivity) > _worry)
		{ // Are we still connected?
			bytestream_t temp;
			ByteStream.init(&temp, 0);
			sendMessage(NETWORK_MESSAGE_HEARTBEAT, -1, connection->id, temp);
		}
	}
	else if (_networkMode == NETWORK_MODE_HOST)
	{
		uint i;
		for (i = 1; i < _maxConnections; i++)
		{
			networkConnection_t *connection = &_connections[i];
			if (connection->socket != INVALID_SOCKET)
			{
				if ((curTime - connection->lastInActivity) > _timeout)
				{ // This client has been idle for too long, drop it
					printf("Dropping idle connection %i\n", connection->id);
					_closeConnection(connection, true);
				}
				else if ((curTime - connection->lastOutActivity) > _worry)
				{ // Can you hear meeeee
					bytestream_t temp;
					ByteStream.init(&temp, 0);
					sendMessage(NETWORK_MESSAGE_HEARTBEAT, -1, connection->id, temp);
				}
			}
		}
	}
}

uint _sizeForNetworkMessage(networkMessage_t message)
{
	return sizeof(uint) + sizeof(message.type) + sizeof(message.senderID) + sizeof(message.receiverID) + sizeof(byte) * message.content.len;
}

void _serializeNetworkMessage(networkMessage_t in, bytestream_t *out)
{
	uint size = _sizeForNetworkMessage(in);
	ByteStream.init(out, size);
	ByteStream.write(out, (byte*)&size, sizeof(uint));
	ByteStream.write(out, (byte*)&in.type, sizeof(in.type));
	ByteStream.write(out, (byte*)&in.senderID, sizeof(in.senderID));
	ByteStream.write(out, (byte*)&in.receiverID, sizeof(in.receiverID));
	ByteStream.write(out, in.content.data, in.content.len);
	out->len = size;
}

bool _doSend(networkConnection_t *connection, bytestream_t serializedMessage)
{
	if (connection->protocol == SOCKET_PROTOCOL_TCP)
	{
		if (!TCP_sendMessage(connection->socket, serializedMessage))
		{
			return false;
		}
	}
	else if (connection->protocol == SOCKET_PROTOCOL_UDP)
	{
		if (!UDP_sendMessage(serializedMessage, connection->socket, (const SOCKADDR*)&connection->address, sizeof(connection->address)))
		{
			return false;
		}
	}
	connection->lastOutActivity = Time.milliseconds();

	return true;
}

void sendMessage(networkMessageType_t type, int senderID, int receiverID, bytestream_t content)
{
	networkConnection_t *target = NULL;
	networkMessage_t message;
	bytestream_t serializedMessage;

	message.type = type;
	message.senderID = senderID;
	message.receiverID = receiverID;

	ByteStream.init(&message.content, content.len);
	ByteStream.write(&message.content, content.data, content.len);
	message.content.cursor = 0;

	if (_networkMode == NETWORK_MODE_HOST)
	{ // Send it directly
		target = _connectionWithID(receiverID);
	}
	else if (_networkMode == NETWORK_MODE_CLIENT)
	{ // Send it to the host
		target = &_connections[0];
	}

	if (target)
	{
		_serializeNetworkMessage(message, &serializedMessage);

		if (_networkMode == NETWORK_MODE_HOST && target->id == _connections[0].id)
		{ // Broadcast to everyone except the sender
			uint i;
			for (i = 1; i < _maxConnections; i++)
			{
				//if (_connections[i].id != message.senderID) // Actually including the sender too for now...
				if (_connections[i].socket != INVALID_SOCKET)
				{
					if (!_doSend(&_connections[i], serializedMessage))
					{
						printf("Failed to send broadcasted message to client %i\n", i);
					}
				}
			}
		}
		else
		{
			if (!_doSend(target, serializedMessage))
			{
				if (_networkMode == NETWORK_MODE_HOST)
				{
					printf("Failed to send message to client %i\n", target->id);
				}
				else
				{
					printf("Failed to send message to server\n");
				}
			}
		}

		ByteStream.free(&serializedMessage);
	}
	else
	{
		printf("Socket error while sending\n");
	}

	ByteStream.free(&message.content);
}

uint _decodeMessage(bytestream_t *in, networkMessage_t *out)
{ // Returns the size of the decoded message
	uint cursor = 0;
	uint size;
	cursor += ByteStream.read(in, (byte*)&size, sizeof(uint));
	cursor += ByteStream.read(in, (byte*)&out->type, sizeof(out->type));
	cursor += ByteStream.read(in, (byte*)&out->senderID, sizeof(out->senderID));
	cursor += ByteStream.read(in, (byte*)&out->receiverID, sizeof(out->receiverID));
	ByteStream.init(&out->content, size - cursor);
	ByteStream.read(in, out->content.data, out->content.len);
	return size;
}

void receiveMessages(networkUpdate_t *update)
{
	uint i;

	update->messages = NULL;
	update->count = 0;
	
	for (i = 0; i < _maxConnections; i++)
	{
		if (_connections[i].socket != INVALID_SOCKET)
		{
			bool gotMessage = false;
			bytestream_t inMessage;
			if (_connections[i].protocol == SOCKET_PROTOCOL_TCP)
			{
				gotMessage = TCP_receiveMessages(_connections[i].socket, &inMessage);
			}
			else if (_connections[i].protocol == SOCKET_PROTOCOL_UDP)
			{
				int len = sizeof(_connections[i].address);
				gotMessage = UDP_receiveMessages(&inMessage, _connections[i].socket, (SOCKADDR*)&_connections[i].address, &len);
			}

			if (gotMessage)
			{ // We may have received more than one message, let's make sure they're all parsed
				networkMessage_t *out;
				uint decoded = 0;

				do {
					update->count++;
					update->messages = (networkMessage_t*)Memory.reallocate(update->messages, sizeof(networkMessage_t) * update->count);

					out = &update->messages[update->count-1];

					decoded += _decodeMessage(&inMessage, out);
				
					if (out->senderID == -1)
					{ // Update the sender ID
						out->senderID = _connections[i].id;
					}

					out->receiveTime = Time.milliseconds();
				} while (decoded < inMessage.len);

				ByteStream.free(&inMessage);

				_connections[i].lastInActivity = out->receiveTime;
			}
		}
	}
}

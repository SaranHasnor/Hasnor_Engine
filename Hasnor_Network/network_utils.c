#include <utils.h>
#include "network_utils.h"

void printError(networkStatus_t status)
{
	if (status.error != NETWORK_ERROR_NONE)
	{
		switch (status.error)
		{
		case NETWORK_ERROR_INIT:
			Console.print("NETWORK_ERROR_INIT");
			break;
		case NETWORK_ERROR_SOCKET:
			Console.print("NETWORK_ERROR_SOCKET");
			break;
		case NETWORK_ERROR_BINDING:
			Console.print("NETWORK_ERROR_BINDING");
			break;
		case NETWORK_ERROR_LISTEN:
			Console.print("NETWORK_ERROR_LISTEN");
			break;
		case NETWORK_ERROR_CONNECT:
			Console.print("NETWORK_ERROR_CONNECT");
			break;
		case NETWORK_ERROR_TIMEOUT:
			Console.print("NETWORK_ERROR_TIMEOUT");
			break;
		case NETWORK_ERROR_KICKED:
			Console.print("NETWORK_ERROR_KICKED");
			break;
		case NETWORK_ERROR_UNAUTHORIZED_ACTION:
			Console.print("NETWORK_ERROR_UNAUTHORIZED_ACTION");
			break;
		case NETWORK_ERROR_UNDOCUMENTED:
			Console.print("NETWORK_ERROR_UNDOCUMENTED");
			break;
		}

		Console.print(" (ID: %i)\n", status.socketError);
	}
}

void _printConnectionName(int id)
{
	if (id == -1)
	{
		Console.print("all");
	}
	else if (id == 0)
	{
		Console.print("server");
	}
	else
	{
		Console.print("client %i", id);
	}
}

void printMessage(networkMessage_t message)
{
	switch (message.type)
	{
	case NETWORK_MESSAGE_CONNECT:
		Console.print("CONNECT (");
		break;
	case NETWORK_MESSAGE_HEARTBEAT:
		Console.print("HEARTBEAT (");
		break;
	case NETWORK_MESSAGE_EXIT:
		Console.print("EXIT (");
		break;
	case NETWORK_MESSAGE_CUSTOM:
	default:
		Console.print("MESSAGE (");
		break;
	}

	_printConnectionName(message.senderID);

	Console.print(" -> ");

	_printConnectionName(message.receiverID);

	if (message.content.len > 0)
	{
		Console.print("): %.*s\n", message.content.len, message.content.data);
	}
	else
	{
		Console.print(")\n");
	}
}
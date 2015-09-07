#include <utils.h>
#include "network_utils.h"

void printError(networkStatus_t status)
{
	if (status.error != NETWORK_ERROR_NONE)
	{
		printf("Error: ");

		switch (status.error)
		{
		case NETWORK_ERROR_INIT:
			printf("NETWORK_ERROR_INIT");
			break;
		case NETWORK_ERROR_SOCKET:
			printf("NETWORK_ERROR_SOCKET");
			break;
		case NETWORK_ERROR_BINDING:
			printf("NETWORK_ERROR_BINDING");
			break;
		case NETWORK_ERROR_LISTEN:
			printf("NETWORK_ERROR_LISTEN");
			break;
		case NETWORK_ERROR_CONNECT:
			printf("NETWORK_ERROR_CONNECT");
			break;
		case NETWORK_ERROR_TIMEOUT:
			printf("NETWORK_ERROR_TIMEOUT");
			break;
		case NETWORK_ERROR_KICKED:
			printf("NETWORK_ERROR_KICKED");
			break;
		case NETWORK_ERROR_UNAUTHORIZED_ACTION:
			printf("NETWORK_ERROR_UNAUTHORIZED_ACTION");
			break;
		case NETWORK_ERROR_UNDOCUMENTED:
			printf("NETWORK_ERROR_UNDOCUMENTED");
			break;
		}

		printf(" (ID: %i)\n", status.socketError);
	}
}

void _printConnectionName(int id)
{
	if (id == -1)
	{
		printf("all");
	}
	else if (id == 0)
	{
		printf("server");
	}
	else
	{
		printf("client %i", id);
	}
}

void printMessage(networkMessage_t message)
{
	switch (message.type)
	{
	case NETWORK_MESSAGE_CONNECT:
		printf("CONNECT (");
		break;
	case NETWORK_MESSAGE_HEARTBEAT:
		printf("HEARTBEAT (");
		break;
	case NETWORK_MESSAGE_EXIT:
		printf("EXIT (");
		break;
	case NETWORK_MESSAGE_CUSTOM:
	default:
		printf("MESSAGE (");
		break;
	}

	_printConnectionName(message.senderID);

	printf(" -> ");

	_printConnectionName(message.receiverID);

	if (message.content.len > 0)
	{
		printf("): %.*s\n", message.content.len, message.content.data);
	}
	else
	{
		printf(")\n");
	}
}
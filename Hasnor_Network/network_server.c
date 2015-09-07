#include "network_server.h"
#include "network.h"

void SV_initServer(int maxConnections, unsigned short port, socketProtocol_t protocol, networkStatus_t *status)
{
	if (currentNetworkMode() != NETWORK_MODE_LOCAL)
	{
		if (status)
		{
			status->error = NETWORK_ERROR_UNAUTHORIZED_ACTION;
			status->socketError = 0;
		}
		return;
	}

	if (!createHostSocket(maxConnections, port, protocol, status))
	{
		return;
	}

	if (status)
	{
		status->error = NETWORK_ERROR_NONE;
		status->socketError = 0;
	}
}

int SV_checkForNewClients()
{ // Returns the amount of new clients
	int count = 0;
	while (getNewClient())
	{
		count++;
	}
	return count;
}

void SV_closeServer(networkStatus_t *status)
{
	if (currentNetworkMode() != NETWORK_MODE_HOST)
	{
		if (status)
		{
			status->error = NETWORK_ERROR_UNAUTHORIZED_ACTION;
			status->socketError = 0;
		}
		return;
	}

	disconnect();
}

void SV_kickClient(int clientID)
{
	dropClient(clientID);
}

void SV_sendMessage(int targetID, string message)
{
	bytestream stream;
	bytestream_init(&stream, message.len);
	bytestream_write(&stream, message.s, message.len);
	sendMessage(NETWORK_MESSAGE_CUSTOM, -1, targetID, stream);
}

void SV_update(networkUpdate_t *update)
{
	uint i;

	receiveMessages(update);

	// Redirect the received messages to targeted clients
	for (i = 0; i < update->count; i++)
	{
		networkMessage_t *message = &update->messages[i];
		if (message->type != NETWORK_MESSAGE_HEARTBEAT)
		{
			sendMessage(message->type, message->senderID, message->receiverID, message->content);

			if (message->type == NETWORK_MESSAGE_EXIT)
			{ // Client left
				dropClient(message->senderID);
			}
		}
	}

	checkForTimeOuts();
}
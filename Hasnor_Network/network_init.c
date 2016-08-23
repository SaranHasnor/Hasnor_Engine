#include "network_init.h"
#include "network.h"
#include "network_internal.h"
#include "network_client.h"
#include "network_server.h"

// Imports from network_utils
void printError(networkStatus_t status);
void printMessage(networkMessage_t message);

void _sendMessage(int targetID, bytestream_t message)
{
	sendMessage(NETWORK_MESSAGE_CUSTOM, -1, targetID, message);
}

void _update(networkUpdate_t *update)
{
	receiveMessages(update);

	if (currentNetworkMode() == NETWORK_MODE_HOST || currentNetworkMode() == NETWORK_MODE_HOST_WITH_MASTER)
	{ // Redirect the received messages to targeted clients
		uint i;
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
	}

	checkForTimeOuts();
}

void initHasnorNetwork(void)
{
	Network.init = setupNetwork;
	Network.shutdown = shutdownNetwork;
	Network.setTimeout = setTimeout;
	Network.sendMessage = _sendMessage;
	Network.update = _update;

	Network.SV.start = SV_initServer;
	Network.SV.stop = SV_closeServer;
	Network.SV.kickClient = SV_kickClient;
	Network.SV.checkForNewClients = SV_checkForNewClients;

	Network.CL.connect = CL_connectToServer;
	Network.CL.disconnect = CL_disconnectFromServer;
	Network.CL.isConnected = CL_connected;

	Network.printError = printError;
	Network.printMessage = printMessage;
}

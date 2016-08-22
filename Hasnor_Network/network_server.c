#include "network_server.h"
#include "network_internal.h"

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

int SV_checkForNewClients(void)
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

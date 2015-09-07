#ifndef _NETWORK_TOOLS_DEFINED
#define _NETWORK_TOOLS_DEFINED

#include <utils.h>
#include "network_utils.h"

typedef enum {
	NETWORK_MODE_LOCAL,
	NETWORK_MODE_CLIENT,
	NETWORK_MODE_HOST,
	NETWORK_MODE_MASTER
} networkMode_t;

typedef enum {
	SOCKET_TYPE_CLIENT,
	SOCKET_TYPE_HOST
} socketType_t;

networkMode_t currentNetworkMode();
uint maxConnections();

void setupNetwork();
void shutdownNetwork();

bool createHostSocket(int maxConnections, unsigned short port, socketProtocol_t protocol, networkStatus_t *status);
bool createSocket(const char *address, unsigned short port, socketProtocol_t protocol, networkStatus_t *status);

bool tryToConnect(bytestream clientInfo, networkStatus_t *status);
bool getNewClient();

void dropClient(int id);
void disconnect();

void checkForTimeOuts();

void sendMessage(networkMessageType_t type, int senderID, int receiverID, bytestream content);
void receiveMessages(networkUpdate_t *update);

#endif
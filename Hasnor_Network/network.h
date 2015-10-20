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
	SOCKET_TYPE_INACTIVE,
	SOCKET_TYPE_CLIENT,
	SOCKET_TYPE_HOST
} socketType_t;

networkMode_t currentNetworkMode(void);
uint maxConnections(void);

void setupNetwork(long worryTime, long timeoutTime);
void shutdownNetwork(void);

bool createHostSocket(int maxConnections, unsigned short port, socketProtocol_t protocol, networkStatus_t *status);
bool createSocket(const char *address, unsigned short port, socketProtocol_t protocol, networkStatus_t *status);

bool tryToConnect(bytestream_t clientInfo, networkStatus_t *status);
bool getNewClient(void);

void dropClient(int id);
void disconnect(void);

void checkForTimeOuts(void);

void sendMessage(networkMessageType_t type, int senderID, int receiverID, bytestream_t content);
void receiveMessages(networkUpdate_t *update);

#endif
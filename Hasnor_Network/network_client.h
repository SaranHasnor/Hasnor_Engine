#include "network_utils.h"

extern void setupNetwork(long worryTime, long timeoutTime);
extern void shutdownNetwork();

void CL_connectToServer(const char *address, unsigned short port, bytestream clientInfo, socketProtocol_t protocol, networkStatus_t *status);
void CL_disconnectFromServer(networkStatus_t *status);

bool CL_connected();

void CL_sendMessage(int targetID, bytestream message);
void CL_update(networkUpdate_t *update);

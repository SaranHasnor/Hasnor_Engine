#include "network_utils.h"

void SV_initServer(int maxConnections, unsigned short port, socketProtocol_t protocol, networkStatus_t *status);
int SV_checkForNewClients(void);
void SV_kickClient(int clientID);
void SV_closeServer(networkStatus_t *status);

#include "network_utils.h"
#include <utils_string.h>

extern void setupNetwork();
extern void shutdownNetwork();

void SV_initServer(int maxConnections, unsigned short port, socketProtocol_t protocol, networkStatus_t *status);
int SV_checkForNewClients();
void SV_kickClient(int clientID);
void SV_closeServer(networkStatus_t *status);

void SV_sendMessage(int targetID, string message);
void SV_update(networkUpdate_t *update);

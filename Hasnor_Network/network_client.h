#include "network_utils.h"

void CL_connectToServer(const char *address, unsigned short port, bytestream_t clientInfo, socketProtocol_t protocol, networkStatus_t *status);
void CL_disconnectFromServer(networkStatus_t *status);

bool CL_connected(void);

#ifndef _NETWORK_IMPORTED
#define _NETWORK_IMPORTED

#include "network_utils.h"

typedef struct {
	void	(*start)(int maxConnections, unsigned short port, socketProtocol_t protocol, networkStatus_t *status);
	void	(*stop)(networkStatus_t *status);

	int		(*checkForNewClients)(void);
	void	(*kickClient)(int clientID);
} _network_server_functions;

typedef struct {
	void	(*connect)(const char *address, unsigned short port, bytestream_t clientInfo, socketProtocol_t protocol, networkStatus_t *status);
	void	(*disconnect)(networkStatus_t *status);

	bool	(*isConnected)(void);
} _network_client_functions;

typedef struct {
	void	(*init)(networkMode_t mode);
	void	(*shutdown)(void);

	void	(*setTimeout)(timestamp_ms_t worryTime, timestamp_ms_t timeoutTime);

	void	(*sendMessage)(int targetID, bytestream_t message);
	void	(*update)(networkUpdate_t *update);

	_network_server_functions SV;
	_network_client_functions CL;

	void	(*printError)(networkStatus_t status);
	void	(*printMessage)(networkMessage_t message);
} _network_functions;

_network_functions Network;

void HasnorNetworkInit(networkMode_t mode);

#endif

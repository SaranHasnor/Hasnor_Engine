#ifndef _NETWORK_UTILS_DEFINED
#define _NETWORK_UTILS_DEFINED

#include <utils_types.h>
#include <utils_bytestream.h>
#include <utils_time.h>

typedef enum {
	NETWORK_MODE_LOCAL,
	NETWORK_MODE_CLIENT,
	NETWORK_MODE_HOST,
	NETWORK_MODE_HOST_WITH_MASTER,
	NETWORK_MODE_P2P
} networkMode_t;

typedef enum {
	NETWORK_ERROR_NONE,

	// Setup errors
	NETWORK_ERROR_INIT,
	NETWORK_ERROR_SOCKET,
	NETWORK_ERROR_BINDING,
	NETWORK_ERROR_LISTEN,
	NETWORK_ERROR_CONNECT,

	// Run-time errors
	NETWORK_ERROR_TIMEOUT,
	NETWORK_ERROR_KICKED,

	// Common errors
	NETWORK_ERROR_UNAUTHORIZED_ACTION,
	
	NETWORK_ERROR_UNDOCUMENTED
} networkError_t;


typedef struct {
	networkError_t		error;
	int					socketError;
} networkStatus_t;

typedef enum {
	NETWORK_MESSAGE_CONNECT,		// Message sent to clients to notify them that a new client has joined
	NETWORK_MESSAGE_HEARTBEAT,		// Empty message sent to the server to remind him we're connected
	NETWORK_MESSAGE_EXIT,			// Message sent to notify the server and clients of a disconnection

	NETWORK_MESSAGE_CUSTOM			// Program-specific messages
} networkMessageType_t;

typedef struct {
	int						senderID;	// If -1, message was sent by the server
	int						receiverID; // If -1, message is a broadcast
	networkMessageType_t	type;
	bytestream_t			content;
	timestamp_ms_t			receiveTime;
} networkMessage_t;

typedef struct {
	networkMessage_t	*messages;
	uint				count;

	networkStatus_t		status;
} networkUpdate_t;

typedef enum {
	SOCKET_PROTOCOL_TCP,
	SOCKET_PROTOCOL_UDP
} socketProtocol_t;

#endif
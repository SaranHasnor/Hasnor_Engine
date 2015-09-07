#include <utils.h>
#include <utils_string.h>
#include <engine_interface.h>
#include <network_client.h>
#include <string.h>

/*
interface_init.c

- Interface creation
*/

#pragma warning (disable:4996)

// Interface variables

char *address;
char *name;
int *port;

char *message;

// End of interface variables

void send(void)
{
	uint len = strlen(message);
	bytestream stream;
	bytestream_init(&stream, len);
	bytestream_write(&stream, (byte*)message, len);
	CL_sendMessage(-1, stream);
	bytestream_destroy(&stream);
	message[0] = '\0';
}

void connect(void)
{
	networkStatus_t status;
	bytestream clientInfo;
	bytestream_init(&clientInfo, strlen(name));
	bytestream_write(&clientInfo, (byte*)name, clientInfo.len);

	CL_connectToServer(address, (unsigned short)*port, clientInfo, SOCKET_PROTOCOL_TCP, &status);

	bytestream_destroy(&clientInfo);

	if (status.error == NETWORK_ERROR_NONE)
	{ // Success
		//setMenu(1);
	}
	else
	{
		printError(status);
	}
}

void leave(void)
{
	CL_disconnectFromServer(NULL);
	//setMenu(0);
}


void createInterface()
{
	int temp;

	address = (char*)mem_alloc(sizeof(char) * 16);
	strcpy(address, "127.0.0.1");
	address[9] = '\0';

	name = (char*)mem_alloc(sizeof(char) * 32);
	strcpy(name, "Client");
	name[6] = '\0';

	message = (char*)mem_alloc(sizeof(char) * 1024);
	message[0] = '\0';

	port = (int*)mem_alloc(sizeof(int));
	*port = 5875;
	
	// Menu 0: Connection
	//newMenu(0, 0, NULL, NULL);

	interface_pushBlock(relativePlacement(0.0f, 0.8f, 1.0f, 0.2f));

	interface_staticLabel(quickString("Connectez-vous a un serveur"), staticPlacement(5, 0, 250, 50), ANCHOR_TOP_LEFT);

	interface_staticLabel(quickString("Votre nom:"),  staticPlacement(5, 35, 200, 50), ANCHOR_TOP_LEFT);
	temp = interface_textField(staticPlacement(100, 30, 150, 30), FIELDTYPE_TEXT);
	interface_setTextFieldValue(temp, 0, 0, (void**)&name, true);

	interface_staticLabel(quickString("Addresse du serveur:"), staticPlacement(275, 35, 200, 50), ANCHOR_TOP_LEFT);
	temp = interface_textField(staticPlacement(450, 30, 300, 30), FIELDTYPE_TEXT);
	interface_setTextFieldValue(temp, 0, 0, (void**)&address, true);
	temp = interface_textField(staticPlacement(775, 30, 100, 30), FIELDTYPE_INT);
	interface_setTextFieldValue(temp, 0, 65535, (void**)&port, true);

	interface_staticButton(quickString("Connexion"), staticPlacement(900, 20, 150, 60), connect);
	
	interface_staticButton(quickString("Quitter"), relativePlacement(0.9f, 0.8f, 0.05f, 0.1f), quit);

	// Menu 1: Chat room
	//newMenu(INTERFACE_WIDTH, 0, NULL, NULL);

	/*temp = interface_textField(1, 50, 30, INTERFACE_WIDTH - 150, 30, FIELDTYPE_TEXT);
	interface_setTextFieldValue(temp, 0, 0, message, true);

	interface_staticButton(quickString("Envoyer"), 1, INTERFACE_WIDTH - 90, 30, 80, 30, send);

	newButton(quickString("Se deconnecter"), 1, INTERFACE_WIDTH - 160, INTERFACE_HEIGHT - 30, 150, 20, leave);*/

	interface_popBlock();

	interface_updateLayout();
}

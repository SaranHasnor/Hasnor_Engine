#include <utils.h>
#include <utils_string.h>
#include <engine.h>
#include <network_client.h>

/*
Engine.UI.init.c

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
	uint len = String.length(message);
	bytestream_t stream;
	ByteStream.init(&stream, len);
	ByteStream.write(&stream, (byte*)message, len);
	CL_sendMessage(-1, stream);
	ByteStream.free(&stream);
	message[0] = '\0';
}

void connect(void)
{
	networkStatus_t status;
	bytestream_t clientInfo;
	ByteStream.init(&clientInfo, String.length(name));
	ByteStream.write(&clientInfo, (byte*)name, clientInfo.len);

	CL_connectToServer(address, (unsigned short)*port, clientInfo, SOCKET_PROTOCOL_TCP, &status);

	ByteStream.free(&clientInfo);

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


void initSampleInterface(void)
{
	int temp;

	address = newArray(char, 16);
	String.copy(address, "127.0.0.1");
	address[9] = '\0';

	name = newArray(char, 32);
	String.copy(name, "Client");
	name[6] = '\0';

	message = newArray(char, 1024);
	message[0] = '\0';

	port = newObject(int);
	*port = 5875;
	
	// Menu 0: Connection
	//newMenu(0, 0, NULL, NULL);

	Engine.UI.pushBlock(relativePlacement(0.0f, 0.8f, 1.0f, 0.2f));

	Engine.UI.staticLabel(String.create("Connectez-vous a un serveur"), staticPlacement(5, 0, 250, 50), ANCHOR_TOP_LEFT);

	Engine.UI.staticLabel(String.create("Votre nom:"),  staticPlacement(5, 35, 200, 50), ANCHOR_TOP_LEFT);
	temp = Engine.UI.textField(staticPlacement(100, 30, 150, 30), FIELDTYPE_TEXT);
	Engine.UI.setTextFieldValue(temp, 0, 0, (void**)&name, true);

	Engine.UI.staticLabel(String.create("Addresse du serveur:"), staticPlacement(275, 35, 200, 50), ANCHOR_TOP_LEFT);
	temp = Engine.UI.textField(staticPlacement(450, 30, 300, 30), FIELDTYPE_TEXT);
	Engine.UI.setTextFieldValue(temp, 0, 0, (void**)&address, true);
	temp = Engine.UI.textField(staticPlacement(775, 30, 100, 30), FIELDTYPE_INT);
	Engine.UI.setTextFieldValue(temp, 0, 65535, (void**)&port, true);

	Engine.UI.staticButton(String.create("Connexion"), staticPlacement(900, 20, 150, 60), connect);
	
	Engine.UI.staticButton(String.create("Quitter"), relativePlacement(0.9f, 0.8f, 0.05f, 0.1f), quit);

	// Menu 1: Chat room
	//newMenu(Engine.UI.WIDTH, 0, NULL, NULL);

	/*temp = Engine.UI.textField(1, 50, 30, Engine.UI.WIDTH - 150, 30, FIELDTYPE_TEXT);
	Engine.UI.setTextFieldValue(temp, 0, 0, message, true);

	Engine.UI.staticButton(String.create("Envoyer"), 1, Engine.UI.WIDTH - 90, 30, 80, 30, send);

	newButton(String.create("Se deconnecter"), 1, Engine.UI.WIDTH - 160, Engine.UI.HEIGHT - 30, 150, 20, leave);*/

	Engine.UI.popBlock();

	Engine.UI.updateLayout();
}

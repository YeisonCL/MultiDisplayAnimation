/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "TransmitterMode.h"
#include "XMLParser.h"

int transmitterSocket;
struct sockaddr_in transmitterAdress;
char **ips;
int *ports;

static void bindSocket();
static void createSocket();
static void createArrayIp(int pConnectionsCounter);
static void loadIPS(int pConnectionsCounter);

void sendFigure(char *pReadBuffer, int pNumberCharacters, int pConnection)
{
	int sentBytes;
	int unsentBytes = pNumberCharacters;

	createSocket();

	transmitterAdress.sin_addr.s_addr = inet_addr(ips[pConnection]);
    transmitterAdress.sin_port = htons(ports[pConnection]);

	if (connect(transmitterSocket, (struct sockaddr*)&transmitterAdress, sizeof(struct sockaddr)) == -1)
	{
		erase();
		printw("Se ha producido un error al intentar conectar con el servicio externo...");
		refresh();
		getch();
		exit(1);
	}
    while (unsentBytes > 0) 
    {
    	sentBytes = send(transmitterSocket, pReadBuffer, unsentBytes, 0);
		if (sentBytes <= 0 ) 
		{
		    if (errno == EINTR)
		    {
		    	sentBytes = 0;
		    }
		    else
		    {
		    	break;
		    }
		}
		unsentBytes -= sentBytes;
		pReadBuffer += sentBytes;
    }
    close(transmitterSocket);
}

void startTransmitterMode(int pConnectionsCounter)
{
	createArrayIp(pConnectionsCounter);
	loadIPS(pConnectionsCounter);
}

void freeMemoryTransmitterMode(int pConnectionsCounter)
{
	for(int i = 0; i < pConnectionsCounter; i++)
	{
		free(ips[i]);
	}
	free(ips);
	free(ports);
}

static void createArrayIp(int pConnectionsCounter)
{
	bindSocket();
	ips = (char**)malloc(sizeof(char*) * pConnectionsCounter);
	ports = (int*)malloc(sizeof(int) * pConnectionsCounter);
}

static void loadIPS(int pConnectionsCounter)
{
	char* stringCharacter = (char*)malloc(32 * sizeof(char));
	for(int i = 1; i <= pConnectionsCounter; i++)
	{
		snprintf(stringCharacter, 32, "%d", i);
		ips[i - 1] = extractIP(createDocumentXML("conexiones.xml"), stringCharacter);
		ports[i - 1] = extractPort(createDocumentXML("conexiones.xml"), stringCharacter);
		memset(stringCharacter, 0, 32);
	}
	free(stringCharacter);
}

static void createSocket()
{
	transmitterSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(transmitterSocket == -1)
	{
		erase();
		printw("Se ha presentado un error al intentar crear el socket de conexión...");
		refresh();
		getch();
		exit(1);
	}
}

static void bindSocket()
{
	transmitterAdress.sin_family = AF_INET;
}
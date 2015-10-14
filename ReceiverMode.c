/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "ReceiverMode.h"

int socketServer;
struct sockaddr_in serverAdress;
int incomingConnection;

static void createSocket();
static void bindSocket(int pPort);
static void listeningConnections();
static void readFigure(int pConnection, char **pReadBuffer, int pMaxRead);
static char **strSplit(char *pFinalString, char *pDelimiter);
static void printObject(char *pObject);
static void readAndPrintFigure(int pConnection);

void startReceiverMode(int pPort)
{
	createSocket();
	bindSocket(pPort);
	listeningConnections();
	erase();
	refresh();
	while(1)
	{
		incomingConnection = accept(socketServer, NULL, NULL);
		readAndPrintFigure(incomingConnection);
		shutdown(incomingConnection, 2);
		close(incomingConnection);
	}
}

static void readAndPrintFigure(int pConnection)
{
	char *figure = calloc(1024, sizeof(char));
	readFigure(pConnection, &figure, 1024);
	printObject(figure);
	free(figure);
}

static void createSocket()
{
	socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if(socketServer == -1)
	{
		erase();
		printw("Se ha presentado un error al intentar crear el socket de conexión...");
		refresh();
		getch();
		exit(1);
	}
}

static void bindSocket(int pPort)
{
	serverAdress.sin_family = AF_INET;
    serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAdress.sin_port = htons(pPort);
    if(bind(socketServer, (struct sockaddr *) &serverAdress, sizeof(serverAdress)) == -1)
    {
    	erase();
		printw("Se ha presentado un error al intentar asociar el socket de conexión...");
		refresh();
		getch();
		exit(1);
    }
}

static void listeningConnections()
{
	if(listen(socketServer, MAX_CONNECTIONS) == -1)
	{
		erase();
		printw("Se ha presentado un error al intentar escuchar conexiones...");
		refresh();
		getch();
		exit(1);
	}
}

static void readFigure(int pConnection, char **pReadBuffer, int pMaxRead)
{
	char *readBufferAux = *pReadBuffer;
	char *readBufferAuxTwo = *pReadBuffer;
	int countRealloc = 2;
	int initialMaxRead = pMaxRead;
	char c;
    for (int i = 0; i < pMaxRead; i++) 
    {
    	int valueAux = recv(pConnection, &c, 1, 0);
		if (valueAux == 1 ) 
		{
		    *readBufferAuxTwo++ = c;
		    if (c == '$')
		    {
		    	break;
		    }
		    if(i + 2 == pMaxRead)
		    {
		    	pMaxRead = countRealloc * initialMaxRead;
		    	char *auxPtr = realloc(readBufferAux, countRealloc * initialMaxRead);
		    	*pReadBuffer = auxPtr;
		    	readBufferAuxTwo = auxPtr;
		    	readBufferAux = auxPtr;
		    	auxPtr = NULL;
		    	readBufferAuxTwo = readBufferAuxTwo + i + 1;
		    	countRealloc++;
		    }
		}
    }
}

static char **strSplit(char *pFinalString, char *pDelimiter) 
{
    int tokensAlloc = 1;
    int tokensUsed = 0;
    char **tokens = calloc(tokensAlloc, sizeof(char*));
    char *token, *strtok_ctx;

    for (token = strtok_r(pFinalString, pDelimiter, &strtok_ctx); token != NULL; token = strtok_r(NULL, pDelimiter, &strtok_ctx)) 
    {
        if (tokensUsed == tokensAlloc) 
        {
            tokensAlloc = tokensAlloc * 2;
            tokens = realloc(tokens, tokensAlloc * sizeof(char*));
        }
        tokens[tokensUsed++] = strdup(token);
    }
    if (tokensUsed == 0) 
    {
        free(tokens);
        tokens = NULL;
    } 
    else 
    {
        tokens = realloc(tokens, tokensUsed * sizeof(char*));
    }
    return tokens;
}

static void printObject(char *pObject)
{
	char **tokens = strSplit(pObject, ";");
	int positionX = atoi(tokens[0]);
	free(tokens[0]);
	int positionY = atoi(tokens[1]);
	free(tokens[1]);
	int width = atoi(tokens[2]);
	free(tokens[2]);
	int height = atoi(tokens[3]);
	free(tokens[3]);
	char *object = strdup(tokens[4]);
	free(tokens[4]);
	if(tokens != NULL)
	{
		free(tokens);
	}
	int counter = 0;
	for(int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			move(positionY + j, positionX + i);
			if(strncmp(&object[counter], "|", 1) != 0)
			{
				printw("%c", object[counter]);
			}
			counter++;
			refresh();
		}
	}
	free(object);
}
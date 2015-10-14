/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "Animation.h"
#include "TransmitterMode.h"
#include "XMLParser.h"

ASCIIFigure animations = NULL;
thread_t *threadsArray;
int animationCounter = 0;
long **positionStructure;
thread_mutex_t generalMutex;
thread_mutex_t useSocket;

static void *animateObject(void *pFigure);
static void setAsciiObject(ASCIIFigure pFigure);
static void initialMutexStructure(ASCIIFigure pFigure);
static void insertAllPositions(ASCIIFigure pFigure);
static void deleteAllPositions(ASCIIFigure pFigure);
static int isBlockedPositions(ASCIIFigure pFigure);
static void makeFigureCopy(ASCIIFigure pFigureCopy, ASCIIFigure pOriginalFigure);
static void loadAsciiObject(char ***pObjectAscii, int pWidth, int pHeight, char *pFileName);
static void checkMovementAngle(ASCIIFigure pFigure, int *pControlSignal);
static void setInitialFigureAngle(ASCIIFigure pFigure);
static void verifyFigureExplosion(ASCIIFigure pFigure);
static void freeAllMemory();
static int calculateRepetitions(ASCIIFigure pFigure);
static void printFigure(ASCIIFigure pFigure, int pErase);
static void prepareFigure(ASCIIFigure pFigure, char *FigureOne, char *pFigureTwo, int pCurrentPosition, int pWidthCurrentCanvas, int pWidthNextCanvas, int pErase);
static void translateMeasures(ASCIIFigure pFigure, char *pFinalString, int pPositionX, int pWidth);
static void translateObject(ASCIIFigure pFigure, char *pFinalString, int pWidth, int pShift, int pErase);
static int detectCurrentCanvas(int pCurrentPosition);
static int getTotalCanvas(int pCanvasSize);
static void initializeSettings();

extern int structuredCreated;
extern TCBQueue threadsQueue;

int structuredMutexCreated = 0;
int quantum = 0;
int figureDelay = 0;
int explosionVelocity = 0;

//DEBUGGING
extern FILE *debuggingFile;
//DEBUGGING

void createArrayAnimation(int pFigureCounter)
{
	animations = (ASCIIFigure)malloc(sizeof(struct ASCIIArt) * pFigureCounter);
	threadsArray = (thread_t*)malloc(sizeof(thread_t) * pFigureCounter);
}

void insertFigureToAnimation(ASCIIFigure pFigure)
{
	pFigure->explosion = (char ***)malloc (3 * sizeof(char **));
	setAsciiObject(pFigure);
	animations[animationCounter++] = *pFigure;
}

static void setAsciiObject(ASCIIFigure pFigure)
{
	char *fileName = (char*)malloc(sizeof(char) * 32);
	strcpy(fileName, "explosion1");
	loadAsciiObject(&pFigure->explosion[0], 11, 5, fileName);
	memset(fileName, 0, 32);
	strcpy(fileName, "explosion2");
	loadAsciiObject(&pFigure->explosion[1], 13, 7, fileName);
	memset(fileName, 0, 32);
	strcpy(fileName, "explosion3");
	loadAsciiObject(&pFigure->explosion[2], 29, 13, fileName);
	memset(fileName, 0, 32);
	if(pFigure->angle[0] == 0 && pFigure->angle[1] == 0)
	{
		strcpy(fileName, pFigure->objectName);
		loadAsciiObject(&pFigure->object0, pFigure->objectSize0[0], pFigure->objectSize0[1], fileName);
	}
	else
	{
		strcpy(fileName, pFigure->objectName);
		loadAsciiObject(&pFigure->object0, pFigure->objectSize0[0], pFigure->objectSize0[1], fileName);
		memset(fileName, 0, 32);
		strcpy(fileName, pFigure->objectName);
		strcat(fileName, "E");
		loadAsciiObject(&pFigure->object90, pFigure->objectSize90[0], pFigure->objectSize90[1], fileName);
		memset(fileName, 0, 32);
		strcpy(fileName, pFigure->objectName);
		strcat(fileName, "S");
		loadAsciiObject(&pFigure->object180, pFigure->objectSize180[0], pFigure->objectSize180[1], fileName);
		memset(fileName, 0, 32);
		strcpy(fileName, pFigure->objectName);
		strcat(fileName, "O");
		loadAsciiObject(&pFigure->object270, pFigure->objectSize270[0], pFigure->objectSize270[1], fileName);
	}
	free(fileName);
	setInitialFigureAngle(pFigure);
}

static void setInitialFigureAngle(ASCIIFigure pFigure)
{
	if(pFigure->angle[0] == 0)
	{
		pFigure->object = pFigure->object0;
		pFigure->objectSize[0] = pFigure->objectSize0[0]; 
		pFigure->objectSize[1] = pFigure->objectSize0[1];
	}
	else if(pFigure->angle[0] == 90)
	{
		pFigure->object = pFigure->object90;
		pFigure->objectSize[0] = pFigure->objectSize90[0]; 
		pFigure->objectSize[1] = pFigure->objectSize90[1];
	}
	else if(pFigure->angle[0] == 180)
	{
		pFigure->object = pFigure->object180;
		pFigure->objectSize[0] = pFigure->objectSize180[0]; 
		pFigure->objectSize[1] = pFigure->objectSize180[1];
	}
	else if(pFigure->angle[0] == 270)
	{
		pFigure->object = pFigure->object270;
		pFigure->objectSize[0] = pFigure->objectSize270[0]; 
		pFigure->objectSize[1] = pFigure->objectSize270[1];
	}
}

static void loadAsciiObject(char ***pObjectAscii, int pWidth, int pHeight, char *pFileName)
{
	*pObjectAscii = (char **)malloc (pWidth * sizeof(char *));
    for (int i = 0; i < pWidth; i++) 
    {
        (*pObjectAscii)[i] = (char *) malloc (pHeight * sizeof(char));
    }
	int figureFile = open(pFileName, O_RDONLY);
	if (figureFile == -1) 
	{
		erase();
		printw("No se encuentra el archivo que contiene el ASCII de la figura.");
		refresh();
		getch();
		exit(1);
	}
	else
	{
		char character;
		int i = 0;
		int j = 0;
		while ((read(figureFile, &character, 1)) != 0)
	    {
	    	if(!strncmp("\n", &character, 1))
	    	{
	    		j++;
	    		i = 0;
	    	}
	    	else
	    	{
	    		strncpy(&((*pObjectAscii)[i++][j]), &character, 1);
	    	}
	    }
	}
	close(figureFile);
}

static void initializeSettings()
{
	quantum = extractSettings(createDocumentXML("configuracion.xml"), 0);
	figureDelay = extractSettings(createDocumentXML("configuracion.xml"), 1);
	explosionVelocity = extractSettings(createDocumentXML("configuracion.xml"), 2);
}

void startAnimation()
{
	erase();
	printw("Esperando que finalicen las animaciones...");
	refresh();
	initializeSettings();
	myThreadInit(quantum);
	myThreadMutexInit(&generalMutex);
	myThreadMutexInit(&useSocket);
	startTransmitterMode(getCounter(createDocumentXML("conexiones.xml")));
	for(int i = 0; i < animationCounter; i++)
	{
		myThreadCreate(&threadsArray[i], animateObject, (void *)&animations[i], animations[i].limitTime[1], animations[i].figureType);
		animations[i].threadID = threadsArray[i];
		animations[i].endAnimation = 0;
	}
	for(int i = 0; i < animationCounter; i++)
	{
		myThreadJoin(threadsArray[i], NULL);
	}
	freeAllMemory();
	freeMemoryTransmitterMode(getCounter(createDocumentXML("conexiones.xml")));
	erase();
	printw("Todos las animaciones han finalizado...");
	printw("\n\nPresione una tecla para continuar");
	refresh();
	getch();
}

static void verifyFigureExplosion(ASCIIFigure pFigure)
{
	TCB thread = searchThread(pFigure->threadID, threadsQueue);
	long timeThreadExecution = myThreadGetTimeExecution(pFigure->threadID);
	int midTime = pFigure->limitTime[1] / 2;
	int threePartTime = (pFigure->limitTime[1] / 4) * 3;
	int octavePartTime = (pFigure->limitTime[1] / 8) * 7;
	int halfway = (pFigure->position[2] - pFigure->position[0]) / 2;
	int threePartRoad = ((pFigure->position[2] - pFigure->position[0]) / 4) * 3; 
	int octavePartRoad = ((pFigure->position[2] - pFigure->position[0]) / 8) * 7;
	//DEBUGGING
    fprintf(debuggingFile,"Animacion: Thread %d tiene un tiempo de ejecucion de %lu\n", (int)pFigure->threadID, timeThreadExecution);
    //DEBUGGING
	if(timeThreadExecution > pFigure->limitTime[1])
	{
		pFigure->endAnimation = 1;
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: El thread %d exploto\n", (int)pFigure->threadID);
	    //DEBUGGING
	}
	else if(timeThreadExecution > (long)midTime && pFigure->currentPosition[0] < halfway && thread->ultimateWarningLevel == 0)
	{
		thread->warningLevel = 1;
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Colocando el thread %d en nivel de peligro 1\n", (int)pFigure->threadID);
	    //DEBUGGING
	}
	else if(timeThreadExecution > (long)threePartTime && pFigure->currentPosition[0] < threePartRoad && thread->ultimateWarningLevel == 1)
	{
		thread->warningLevel = 2;
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Colocando el thread %d en nivel de peligro 2\n", (int)pFigure->threadID);
	    //DEBUGGING
	}
	else if(timeThreadExecution > (long)octavePartTime && pFigure->currentPosition[0] < octavePartRoad && thread->ultimateWarningLevel == 2)
	{
		thread->warningLevel = 3;
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Colocando el thread %d en nivel de peligro 3\n", (int)pFigure->threadID);
	    //DEBUGGING
	}
}

static void freeAllMemory()
{
	int width;
	structuredCreated = 0;
	structuredMutexCreated = 0;
	free(threadsArray);
	for(int i = 0; i < animationCounter; i++)
	{
		width = animations[i].canvasSize[0];
		free(animations[i].figureType);
		free(animations[i].objectName);
		for (int j = 0; j < 11; j++) 
    	{
        	free(animations[i].explosion[0][j]);
    	}
    	free(animations[i].explosion[0]);
	    for (int j = 0; j < 13; j++) 
    	{
        	free(animations[i].explosion[1][j]);
    	}
    	free(animations[i].explosion[1]);
	    for (int j = 0; j < 29; j++) 
    	{
        	free(animations[i].explosion[2][j]);
    	}
    	free(animations[i].explosion[2]);
    	free(animations[i].explosion);
		if(animations[i].angle[0] != 0 || animations[i].angle[1] != 0)
		{
			for (int j = 0; j < animations[i].objectSize0[0]; j++) 
	    	{
	        	free(animations[i].object0[j]);
	    	}
	    	free(animations[i].object0);
	    	for (int j = 0; j < animations[i].objectSize90[0]; j++) 
	    	{
	        	free(animations[i].object90[j]);
	    	}
	    	free(animations[i].object90);
	    	for (int j = 0; j < animations[i].objectSize180[0]; j++) 
	    	{
	        	free(animations[i].object180[j]);
	    	}
	    	free(animations[i].object180);
	    	for (int j = 0; j < animations[i].objectSize270[0]; j++) 
	    	{
	        	free(animations[i].object270[j]);
	    	}
	    	free(animations[i].object270);
		}
		else
		{
			for (int j = 0; j < animations[i].objectSize0[0]; j++) 
	    	{
	        	free(animations[i].object0[j]);
	    	}
	    	free(animations[i].object0);
		}
	}
	free(animations);
	for (int i = 0; i < width; i++) 
	{
    	free(positionStructure[i]);
	}
	free(positionStructure);
	animationCounter = 0;
}

static void initialMutexStructure(ASCIIFigure pFigure)
{
    if(!structuredMutexCreated)
    {
    	int width = pFigure->canvasSize[0];
    	int height = pFigure->canvasSize[1];
    	positionStructure = (long **)calloc (width, sizeof(long *));
	    for (int i = 0; i < width; i++) 
	    {
	        positionStructure[i] = (long *) calloc (height, sizeof(long));
	    }
	    structuredMutexCreated = 1;
	    //DEBUGGING
	    fprintf(debuggingFile,"Animacion: Thread %d creo la estructura de datos\n", (int)myThreadSelf());
	    //DEBUGGING
    }
}

static void insertAllPositions(ASCIIFigure pFigure)
{
	long threadID = myThreadSelf();
	for (int j = 0; j < pFigure->objectSize[1]; j++)
	{
		for(int i = 0; i < pFigure->objectSize[0]; i++)
		{
			if(strncmp(&pFigure->object[i][j], "|", 1) != 0 && (pFigure->currentPosition[0] + i) < pFigure->canvasSize[0] && (pFigure->currentPosition[1] + j) < pFigure->canvasSize[1])
			{
				positionStructure[pFigure->currentPosition[0] + i][pFigure->currentPosition[1] + j] = threadID;
				//DEBUGGING
			    fprintf(debuggingFile,"Animacion: Insertando posicion %d %d para thread %d\n", pFigure->currentPosition[0] + i, pFigure->currentPosition[1] + j, (int)threadID);
			    //DEBUGGING
			}
		}
	}
}

static void deleteAllPositions(ASCIIFigure pFigure)
{
	long threadID = myThreadSelf();
	for (int j = 0; j < pFigure->objectSize[1]; j++)
	{
		for(int i = 0; i < pFigure->objectSize[0]; i++)
		{
			if((pFigure->currentPosition[0] + i) < pFigure->canvasSize[0] && (pFigure->currentPosition[1] + j) < pFigure->canvasSize[1] && positionStructure[pFigure->currentPosition[0] + i][pFigure->currentPosition[1] + j] == threadID)
			{
				positionStructure[pFigure->currentPosition[0] + i][pFigure->currentPosition[1] + j] = 0;
				//DEBUGGING
			    fprintf(debuggingFile,"Animacion: Borrando posicion %d %d para thread %d\n", pFigure->currentPosition[0] + i, pFigure->currentPosition[1] + j, (int)threadID);
			    //DEBUGGING
			}
		}
	}
}

static int isBlockedPositions(ASCIIFigure pFigure)
{
	int result = 0;
	long threadID = myThreadSelf();
	for (int j = 0; j < pFigure->objectSize[1]; j++)
	{
		for(int i = 0; i < pFigure->objectSize[0]; i++)
		{
			if(strncmp(&pFigure->object[i][j], "|", 1) != 0 && (pFigure->currentPosition[0] + i) < pFigure->canvasSize[0] && (pFigure->currentPosition[1] + j) < pFigure->canvasSize[1])
			{
				//DEBUGGING
			    fprintf(debuggingFile,"Animacion: Verificando posicion %d %d para thread %d\n", pFigure->currentPosition[0] + i, pFigure->currentPosition[1] + j, (int)threadID);
			    //DEBUGGING
		    	int valuePosition = positionStructure[pFigure->currentPosition[0] + i][pFigure->currentPosition[1] + j];
				if(valuePosition != 0 && valuePosition != threadID)
				{
					result = 1;
					//DEBUGGING
				    fprintf(debuggingFile,"Animacion: Thread %d tiene restriccion de movimiento en sus posiciones\n", (int)threadID);
				    //DEBUGGING
				}
				if(result)
				{
					break;
				}
			}
		}
		if(result)
		{
			break;
		}
	}
	return result;
}

static void checkMovementAngle(ASCIIFigure pFigure, int *pControlSignal)
{
	if(pFigure->angle[0] != pFigure->angle[1])
	{
		int previousAngle = pFigure->currentAngle;
		if(previousAngle == 0 && *pControlSignal)
		{
			pFigure->object = pFigure->object90;
			pFigure->objectSize[0] = pFigure->objectSize90[0]; 
			pFigure->objectSize[1] = pFigure->objectSize90[1];
			pFigure->currentAngle = 90;
			*pControlSignal = 0;
		}
		else if(previousAngle == 90 && *pControlSignal)
		{
			pFigure->object = pFigure->object180;
			pFigure->objectSize[0] = pFigure->objectSize180[0]; 
			pFigure->objectSize[1] = pFigure->objectSize180[1];
			pFigure->currentAngle = 180;
			*pControlSignal = 0;
		}
		else if(previousAngle == 180 && *pControlSignal)
		{
			pFigure->object = pFigure->object270;
			pFigure->objectSize[0] = pFigure->objectSize270[0]; 
			pFigure->objectSize[1] = pFigure->objectSize270[1];
			pFigure->currentAngle = 270;
			*pControlSignal = 0;
		}
		else if(previousAngle == 270 && *pControlSignal)
		{
			pFigure->object = pFigure->object0;
			pFigure->objectSize[0] = pFigure->objectSize0[0]; 
			pFigure->objectSize[1] = pFigure->objectSize0[1];
			pFigure->currentAngle = 0;
			*pControlSignal = 0;
		}
	}
}

static void makeFigureCopy(ASCIIFigure pFigureCopy, ASCIIFigure pOriginalFigure)
{
	pFigureCopy->objectSize[0] = pOriginalFigure->objectSize[0];
	pFigureCopy->objectSize[1] = pOriginalFigure->objectSize[1];
	pFigureCopy->canvasSize[0] = pOriginalFigure->canvasSize[0];
	pFigureCopy->canvasSize[1] = pOriginalFigure->canvasSize[1];
	pFigureCopy->currentPosition[0] = pOriginalFigure->currentPosition[0];
	pFigureCopy->currentPosition[1] = pOriginalFigure->currentPosition[1];
	pFigureCopy->object = pOriginalFigure->object;
}

static int calculateRepetitions(ASCIIFigure pFigure)
{
	int repetitions = abs(pFigure->angle[1] - pFigure->angle[0]) / 90;
	if(repetitions == 3)
	{
		repetitions = repetitions - 2;
	}
    return repetitions;
}

static void startExplosion(ASCIIFigure pFigure, ASCIIFigure pFigureCopy)
{
	deleteAllPositions(pFigureCopy);
	myThreadMutexUnlock(&generalMutex);
	//DEBUGGING
	fprintf(debuggingFile,"Animacion: Thread %d libera el generalMutex en el comienzo de la explosion\n", (int)myThreadSelf());
	//DEBUGGING
	printFigure(pFigureCopy, 1);
	pFigure->object = pFigure->explosion[0];
	pFigure->objectSize[0] = 11;
	pFigure->objectSize[1] = 5;
	printFigure(pFigure, 0);
	myThreadSleep(explosionVelocity);
	printFigure(pFigure, 1);
	pFigure->object = pFigure->explosion[1];
	pFigure->objectSize[0] = 13;
	pFigure->objectSize[1] = 7;
	printFigure(pFigure, 0);
	myThreadSleep(explosionVelocity);
	printFigure(pFigure, 1);
	pFigure->object = pFigure->explosion[2];
	pFigure->objectSize[0] = 29;
	pFigure->objectSize[1] = 13;
	printFigure(pFigure, 0);
	myThreadSleep(explosionVelocity);
	printFigure(pFigure, 1);
}

static void *animateObject(void *pFigure)
{
	ASCIIFigure figure = (ASCIIFigure)pFigure;
	//DEBUGGING
	long threadID = myThreadSelf();
    fprintf(debuggingFile,"Animacion: Entrando a la animacion el thread %d\n", (int)threadID);
    //DEBUGGING
    myThreadSleep(figure->limitTime[0]);
	ASCIIFigure figureCopy = (ASCIIFigure)malloc(sizeof(struct ASCIIArt));
	int movementYCalc = (abs(figure->position[2] - figure->position[0]))/((abs(figure->position[3] - figure->position[1])) + 1);
	int movementXCount = 0;
	//DEBUGGING
    fprintf(debuggingFile,"Animacion: Thread %d tiene un movimiento en Y de %d\n", (int)threadID, movementYCalc);
    fprintf(debuggingFile,"Animacion: Thread %d solicita el generalMutex\n", (int)threadID);
    //DEBUGGING
	myThreadMutexLock(&generalMutex);
	//DEBUGGING
	fprintf(debuggingFile,"Animacion: Thread %d obtiene el generalMutex\n", (int)threadID);
	//DEBUGGING
	initialMutexStructure(pFigure);
	myThreadMutexUnlock(&generalMutex);
	//DEBUGGING
	fprintf(debuggingFile,"Animacion: Thread %d libera el generalMutex\n", (int)threadID);
	//DEBUGGING
	int repetitions = calculateRepetitions(figure);
	int repetitionsCounter = 0;
	int controlSignal = 0;
	makeFigureCopy(figureCopy, figure);
	while((figure->currentPosition[0] != figure->position[2]) || (figure->currentPosition[1] != figure->position[3]))
	{
		if(repetitionsCounter >= repetitions)
		{
			checkMovementAngle(figure, &controlSignal);
		}
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Thread %d solicita el generalMutex\n", (int)threadID);
	    //DEBUGGING
		myThreadMutexLock(&generalMutex);
		//DEBUGGING
		fprintf(debuggingFile,"Animacion: Thread %d obtiene el generalMutex\n", (int)threadID);
		//DEBUGGING
		if(figure->endAnimation == 1)
		{
			startExplosion(figure, figureCopy);
			break;
		}
		else if(!isBlockedPositions(figure))
		{
			//DEBUGGING
		    fprintf(debuggingFile,"Animacion: Thread %d entro a la animacion con posiciones de %d en XInicial, %d en YInicial, %d en XFinal y %d en YFinal\n", (int)threadID, figure->currentPosition[0], figure->currentPosition[1], figure->position[2], figure->position[3]);
		    //DEBUGGING
			deleteAllPositions(figureCopy);
			insertAllPositions(figure);
			myThreadMutexUnlock(&generalMutex);
			//DEBUGGING
			fprintf(debuggingFile,"Animacion: Thread %d libera el generalMutex\n", (int)threadID);
			//DEBUGGING
			printFigure(figureCopy, 1);
			printFigure(figure, 0);
			myThreadSleep(figureDelay);
			repetitionsCounter++;
			controlSignal = 1;
			makeFigureCopy(figureCopy, figure);
			if(figure->currentPosition[0] < figure->position[2])
			{
				figure->currentPosition[0]++;
				movementXCount++;
				//DEBUGGING
			    fprintf(debuggingFile,"Animacion: Aumentado X para thread %d\n", (int)threadID);
			    //DEBUGGING
			}
			else if(figure->currentPosition[0] > figure->position[2])
			{
				figure->currentPosition[0]--;
				movementXCount++;
				//DEBUGGING
			    fprintf(debuggingFile,"Animacion: Decrementando X para thread %d\n", (int)threadID);
			    //DEBUGGING
			}
			if(figure->currentPosition[1] < figure->position[3] && movementXCount == movementYCalc)
			{
				figure->currentPosition[1]++;
				movementXCount = 0;
				//DEBUGGING
			    fprintf(debuggingFile,"Animacion: Aumentado Y para thread %d\n", (int)threadID);
			    //DEBUGGING
			}
			else if(figure->currentPosition[1] > figure->position[3] && movementXCount == movementYCalc)
			{
				figure->currentPosition[1]--;
				movementXCount = 0;
				//DEBUGGING
			    fprintf(debuggingFile,"Animacion: Decrementando Y para thread %d\n", (int)threadID);
			    //DEBUGGING
			}
			//DEBUGGING
		    fprintf(debuggingFile,"Animacion: Saliendo de la animacion\n");
		    //DEBUGGING
		    verifyFigureExplosion(figure);
		    //DEBUGGING
		    fprintf(debuggingFile,"Animacion: Se verifico la explosion\n");
		    //DEBUGGING
		    myThreadSleep(quantum);
		}
		else
		{
			myThreadMutexUnlock(&generalMutex);
			//DEBUGGING
			fprintf(debuggingFile,"Animacion: Thread %d libera el generalMutex\n", (int)threadID);
			//DEBUGGING
			verifyFigureExplosion(figure);
			myThreadSleep(quantum);
		}
	}
	if(!figure->endAnimation)
	{
		figure->endAnimation = 1;
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Thread %d solicita el generalMutex\n", (int)threadID);
	    //DEBUGGING
		myThreadMutexLock(&generalMutex);
		//DEBUGGING
		fprintf(debuggingFile,"Animacion: Thread %d obtiene el generalMutex\n", (int)threadID);
		//DEBUGGING
		deleteAllPositions(figureCopy);
		myThreadMutexUnlock(&generalMutex);
		//DEBUGGING
		fprintf(debuggingFile,"Animacion: Thread %d libera el generalMutex\n", (int)threadID);
		//DEBUGGING
		printFigure(figureCopy, 1);
	}
	free(figureCopy);
	return 0;
}

static int detectCurrentCanvas(int pCurrentPosition)
{
	return (int)(pCurrentPosition / 80);
}

static int getTotalCanvas(int pCanvasSize)
{
	return (int)(pCanvasSize / 80);
}

static void translateObject(ASCIIFigure pFigure, char *pFinalString, int pWidth, int pShift, int pErase)
{
	for(int i = pShift; i < pWidth; i++)
	{
		for (int j = 0; j < pFigure->objectSize[1]; j++)
		{
			if(pErase)
			{
				if(strncmp(&pFigure->object[i][j], "|", 1) != 0)
				{
					strncat(pFinalString, " ", 1);
				}
				else
				{
					strncat(pFinalString, &pFigure->object[i][j], 1);
				}	
			}
			else
			{
				strncat(pFinalString, &pFigure->object[i][j], 1);
			}
		}
	}
}

static void translateMeasures(ASCIIFigure pFigure, char *pFinalString, int pPositionX, int pWidth)
{
	char* stringCharacter = (char*)malloc(16 * sizeof(char));
	snprintf(stringCharacter, 16, "%d", pPositionX);
	strcpy(pFinalString, stringCharacter);
	strncat(pFinalString, ";", 1);
	snprintf(stringCharacter, 16, "%d", pFigure->currentPosition[1]);
	strcat(pFinalString, stringCharacter);
	strncat(pFinalString, ";", 1);
	snprintf(stringCharacter, 16, "%d", pWidth);
	strcat(pFinalString, stringCharacter);
	strncat(pFinalString, ";", 1);
	snprintf(stringCharacter, 16, "%d", pFigure->objectSize[1]);
	strcat(pFinalString, stringCharacter);
	strncat(pFinalString, ";", 1);
	free(stringCharacter);
}

static void prepareFigure(ASCIIFigure pFigure, char *pFigureOne, char *pFigureTwo, int pCurrentPosition, int pWidthCurrentCanvas, int pWidthNextCanvas, int pErase)
{
	if(pWidthCurrentCanvas == 0 && pWidthNextCanvas == 0)
	{
		translateMeasures(pFigure, pFigureOne, pCurrentPosition, pFigure->objectSize[0]);
		translateObject(pFigure, pFigureOne, pFigure->objectSize[0], 0, pErase);
		strcat(pFigureOne, "$");
	}
	else
	{
		translateMeasures(pFigure, pFigureOne, pCurrentPosition, pWidthCurrentCanvas);
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Primer translate %s\n", pFigureOne);
	    //DEBUGGING
		translateObject(pFigure, pFigureOne, pWidthCurrentCanvas, 0, pErase);
		strcat(pFigureOne, "$");
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Primer translate object %s\n", pFigureOne);
	    //DEBUGGING
		translateMeasures(pFigure, pFigureTwo, 0, pWidthNextCanvas);
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Segundo translate %s\n", pFigureTwo);
	    //DEBUGGING
		translateObject(pFigure, pFigureTwo, pFigure->objectSize[0], pFigure->objectSize[0] - pWidthNextCanvas, pErase);
		strcat(pFigureTwo, "$");
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Segundo translate object %s\n", pFigureTwo);
	    //DEBUGGING
	}
}

static void printFigure(ASCIIFigure pFigure, int pErase)
{
	//DEBUGGING
	fprintf(debuggingFile,"Animacion: Thread %d entro al printFigure\n", (int)pFigure->threadID);
	//DEBUGGING
	int currentCanvas = detectCurrentCanvas(pFigure->currentPosition[0]);
	int currentPosition = pFigure->currentPosition[0] - (currentCanvas * 80);
	if((currentPosition + pFigure->objectSize[0]) > 79)
	{	
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: Thread %d tine una posicion actual de %d y un tamaño de %d\n", (int)pFigure->threadID, currentPosition, pFigure->objectSize[0]);
	    //DEBUGGING
		int widthCurrentCanvas = pFigure->objectSize[0] - ((currentPosition + pFigure->objectSize[0]) - 79);
		int widthNextCanvas = (currentPosition + pFigure->objectSize[0]) - 79;
		//DEBUGGING
	    fprintf(debuggingFile,"Animacion: widthCurrentCanvas es %d y widthNextCanvas es %d\n", widthCurrentCanvas, widthNextCanvas);
	    //DEBUGGING
		char *figureOne = (char*)malloc(sizeof(char) * ((widthCurrentCanvas * pFigure->objectSize[1]) + 64));	
		char *figureTwo = (char*)malloc(sizeof(char) * ((widthNextCanvas * pFigure->objectSize[1]) + 64));
		prepareFigure(pFigure, figureOne, figureTwo, currentPosition, widthCurrentCanvas, widthNextCanvas, pErase);

		if((currentCanvas + 1) == getTotalCanvas(pFigure->canvasSize[0]))
		{
			myThreadMutexLock(&useSocket);
			sendFigure(figureOne, strlen(figureOne), currentCanvas);
			myThreadMutexUnlock(&useSocket);
		}
		else
		{
			myThreadMutexLock(&useSocket);
			sendFigure(figureOne, strlen(figureOne), currentCanvas);
			sendFigure(figureTwo, strlen(figureTwo), currentCanvas + 1);
			myThreadMutexUnlock(&useSocket);
		}
		free(figureOne);
		free(figureTwo);
	}
	else
	{
		char *figureOne = (char*)malloc(sizeof(char) * ((pFigure->objectSize[0] * pFigure->objectSize[1]) + 64));	
		prepareFigure(pFigure, figureOne, NULL, currentPosition, 0, 0, pErase);
		myThreadMutexLock(&useSocket);
		sendFigure(figureOne, strlen(figureOne), currentCanvas);
		myThreadMutexUnlock(&useSocket);
		free(figureOne);
	}
}


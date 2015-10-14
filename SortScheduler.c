/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "SortScheduler.h"
#include "MyThread.h"

extern TCBQueue threadsQueue;
extern struct itimerval timeQuantum;
extern long timeInterval;
extern sigset_t sigProcMask;


//DEBUGGING
extern FILE *debuggingFile;
static void printArray(int *pGenerateThreads, int pInitialSortThreads);
//DEBUGGING

static TCB getSortHead(TCBQueue pQueue);
static void deleteValueThreadGenerates(int *pGenerateThreads, int pValue, int pInitialSortThreads);
static void insertValueThreadGenerates(int *pGenerateThreads, int pValue, int pInitialSortThreads);
static int existInThreadGenerates(int *pGenerateThreads, int pValue, int pInitialSortThreads);
static int generateNewTicket(TCBQueue pQueue, int *pGenerateThreads, int pInitialSortThreads);
static int getThreadGeneratesCount(int *pGenerateThreads, int pInitialSortThreads);
static int detectAllThreadsBlocked(TCBQueue pQueue);

int randomTicket = 0;

void sort()
{
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    TCB currentThread = getSortHead(threadsQueue);
    TCB previousThread = threadsQueue->currentThread;
    if(previousThread == NULL && currentThread == NULL)
    {
        //Deadlock
        fprintf(debuggingFile,"Sorteo: Se ha presentado un error no controlado, posible DEADLOCK entre los hilos creados.\n"); 
        fflush(debuggingFile);
        exit(1);
    }
    else if(currentThread == NULL)
    {
        //DEBUGGING
        fprintf(debuggingFile,"Sorteo: currentThread presenta un valor de NULL\n");
        fflush(debuggingFile);
        //DEBUGGING
        //Todos los threads existentes administrados por el scheduler tipo Sorteo estan bloqueados, probablemente a la espera de que se desbloque uno de tipo RoundRobin o bien 
        //ya no existen threads administrados por Sorteo para colocar.
        threadsQueue->currentThreadCopy = threadsQueue->currentThread;
        threadsQueue->currentThread = NULL;
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        myThreadYield();
    }
    else if(previousThread == NULL && currentThread != threadsQueue->currentThreadCopy)
    {
        //Es posible que el scheduler tipo RoundRobin se quedara sin threads para colocar o tenga todos bloqueados a la espera que se libere uno de tipo Sorteo, 
        //o bien que el thread anterior tambien fuese de tipo Sorteo y ya estuviera completo.
        threadsQueue->currentThread = currentThread;
        timeQuantum.it_value.tv_usec = timeInterval;
        if(threadsQueue->currentThreadCopy != NULL)
        {
            //DEBUGGING
            fprintf(debuggingFile, "Sorteo: previousThread presenta un valor de NULL, haciendo el cambio de contexto, remplazando thread %d por thread %d\n", (int)threadsQueue->currentThreadCopy->threadID, (int)currentThread->threadID );
            fflush(debuggingFile);
            //DEBUGGING
            TCB auxTCB = threadsQueue->currentThreadCopy;
            threadsQueue->currentThreadCopy = NULL;
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            swapcontext(&(auxTCB->threadContext), &(currentThread->threadContext));
        }
        else
        {
            //DEBUGGING
            fprintf(debuggingFile,"Sorteo: previousThread presenta un valor de NULL, colocando en contexto el thread %d\n", (int)currentThread->threadID);
            fflush(debuggingFile);
            //DEBUGGING
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            setcontext(&(currentThread->threadContext));
        }
    }
    else
    {
        if(previousThread != NULL && previousThread != currentThread)
        {
        	//DEBUGGING
            fprintf(debuggingFile,"Sorteo: Haciendo el cambio de contexto, remplazando thread %d por thread %d\n", (int)previousThread->threadID, (int)currentThread->threadID);
            fflush(debuggingFile);
            //DEBUGGING
            //El anterior puede ser tipo Sorteo o RoundRobin pero deben ser diferentes a currentThread pues si es igual quiere decir que solo existe un thread en ejecucion en toda la progra,
            //probablemente el main.
            threadsQueue->currentThread = currentThread;
            threadsQueue->currentThreadCopy = NULL;
            timeQuantum.it_value.tv_usec = timeInterval;
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            swapcontext(&(previousThread->threadContext), &(currentThread->threadContext));
        }
        else
        {
            threadsQueue->currentThread = currentThread;
        }
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    }
}

static void printArray(int *pGenerateThreads, int pInitialSortThreads)
{
	int count = 0;
	while(count < pInitialSortThreads)
	{
		fprintf(debuggingFile,"%d\t", pGenerateThreads[count]);
		count++;
	}
	fprintf(debuggingFile,"\n");
}

static void deleteValueThreadGenerates(int *pGenerateThreads, int pValue, int pInitialSortThreads)
{
	int count = 0;
	while(count < pInitialSortThreads)
	{
		if(pGenerateThreads[count] == pValue)
		{
			pGenerateThreads[count] = 0;
			break;
		}
		else
		{
			count++;
		}
	}
	printArray(pGenerateThreads, pInitialSortThreads);
}

static void insertValueThreadGenerates(int *pGenerateThreads, int pValue, int pInitialSortThreads)
{
	int count = 0;
	while(count < pInitialSortThreads)
	{
		if(pGenerateThreads[count] == 0 && !existInThreadGenerates(pGenerateThreads, pValue, pInitialSortThreads))
		{
			pGenerateThreads[count] = pValue;
			break;
		}
		else
		{
			count++;
		}
	}
	printArray(pGenerateThreads, pInitialSortThreads);
}

static int getThreadGeneratesCount(int *pGenerateThreads, int pInitialSortThreads)
{
	int countArray = 0;
	int count = 0;
	while(countArray < pInitialSortThreads)
	{
		if(pGenerateThreads[countArray] != 0)
		{
			count++;
			countArray++;
		}
		else
		{
			countArray++;
		}
	}
	return count;
}

static int existInThreadGenerates(int *pGenerateThreads, int pValue, int pInitialSortThreads)
{
	int count = 0;
	int result = 0;
	while(count < pInitialSortThreads)
	{
		if(pGenerateThreads[count] == pValue)
		{
			result = 1;
			break;
		}
		else
		{
			count++;
		}
	}
	return result;
}

static int detectAllThreadsBlocked(TCBQueue pQueue)
{
	int result = 1;
	TCB headThread = getHeadTCB(pQueue);
    TCB nextThread = headThread->nextThread;
    if(headThread->sort && !headThread->threadBlocked)
    {
    	result = 0;
    }
    else
    {
    	while(nextThread != headThread) 
	    {
	        if(nextThread->sort && !nextThread->threadBlocked)
	        {
	            result = 0;
	            break;
	        }
	        else
	        {
	            nextThread = nextThread->nextThread;
	        }
	    }
    }
    return result;
}

static int generateNewTicket(TCBQueue pQueue, int *pGenerateThreads, int pInitialSortThreads)
{
	int randomTicket = 0;
	do
	{
		randomTicket = ((rand() % (searchEndTicket(pQueue) - 1)) + 1);
	}while(existInThreadGenerates(pGenerateThreads, (int)searchThreadTicket(randomTicket, pQueue)->threadID, pInitialSortThreads));
	return randomTicket;
}

//DEBE DAR UN THREAD QUE SEA VALIDO, ES DECIR, QUE NO ESTE BLOQUEADO O TERMINADO O SEA DE TIPO ROUNDROBIN, EN CASO DE QUE NO HAYA ALGUNO DEBE RETORNAR NULL,
//ADEMAS TAMBIEN DEBE BORRAR LOS THREADS QUE YA ESTEN COMPLETOS.
static TCB getSortHead(TCBQueue pQueue)
{
	//DEBUGGING
	//printQueue(pQueue);
	//printTickets(pQueue);
	//DEBUGGING
	TCB result = NULL;
	int initialSortThreads = getTCBSortCount(pQueue);
	//DEBUGGING
	fprintf(debuggingFile,"Sorteo: La cantidad de threads tipo sort existentes es %d\n", initialSortThreads);
	//DEBUGGING
	int *generateThreads = (int*)calloc(initialSortThreads, sizeof(int));
	randomTicket = generateNewTicket(pQueue, generateThreads, initialSortThreads);
	//DEBUGGING
	fprintf(debuggingFile,"Sorteo: Generando tickete inicial el cual corresponde a %d\n", randomTicket);
	//DEBUGGING
	TCB headThread = searchThreadTicket(randomTicket, pQueue);
    //DEBUGGING
    fprintf(debuggingFile,"Sorteo: Previo al while el thread es %d\n", (int)headThread->threadID);
    //DEBUGGING
    if(detectAllThreadsBlocked(pQueue))
    {
    	//DEBUGGING
        fprintf(debuggingFile,"Sorteo: Todos los threads del scheduler sorteo se encuentran bloqueados\n");
        //DEBUGGING
        randomTicket = 0;
        result = NULL;
    }
    else
    {
    	while(getThreadGeneratesCount(generateThreads, initialSortThreads) <= getTCBSortCount(pQueue))
	    {
	        //DEBUGGING
	        fprintf(debuggingFile,"Sorteo: Dentro del while la cabeza es el thread %d\n", (int)headThread->threadID);
	        //DEBUGGING
	        if(headThread->threadCompleted && headThread->sort)
	        {
	            //DEBUGGING
	            fprintf(debuggingFile,"Sorteo: Thread numero %d completado por ende se va a eliminar\n", (int)headThread->threadID);
	            //printQueue(pQueue);
	            //DEBUGGING
	            if(pQueue->currentThread == headThread)
	            {
	                pQueue->currentThread = NULL;
	            }
	            deleteValueThreadGenerates(generateThreads, (int)headThread->threadID, initialSortThreads);
	            deleteSortThread(headThread->threadID, pQueue); 
	            //
	            //printTickets(pQueue);
	            //
	            if(getTCBSortCount(pQueue) == 0 || detectAllThreadsBlocked(pQueue))
		        {
		            //DEBUGGING
		            fprintf(debuggingFile,"Sorteo: Se han acabado los threads que administra el scheduler sorteo en la cola o bien todos los threads se encuentran bloqueados\n");
		            //DEBUGGING
		            randomTicket = 0;
		            result = NULL;
		            break;
		        }
		        else
		        {
		        	randomTicket = generateNewTicket(pQueue, generateThreads, initialSortThreads);
		            //DEBUGGING
			        fprintf(debuggingFile,"Sorteo: El tickete random generado es %d\n", randomTicket);
			        //DEBUGGING
		        	headThread = searchThreadTicket(randomTicket, pQueue);
		        	insertValueThreadGenerates(generateThreads, (int)headThread->threadID, initialSortThreads);
		        	//DEBUGGING
		        	//printQueue(pQueue);
		        	//DEBUGGING
		        }
	        }
	        else if(headThread->threadBlocked || headThread->roundRobin)
	        {
	            //DEBUGGING
	            fprintf(debuggingFile,"Sorteo: Thread %d bloqueado o es administrado por el scheduler tipo RoundRobin\n", (int)headThread->threadID);
	            //DEBUGGING
	            randomTicket = generateNewTicket(pQueue, generateThreads, initialSortThreads);
	            //DEBUGGING
		        fprintf(debuggingFile,"Sorteo: El tickete random generado es %d\n", randomTicket);
		        //DEBUGGING
	        	headThread = searchThreadTicket(randomTicket, pQueue);
	        	insertValueThreadGenerates(generateThreads, (int)headThread->threadID, initialSortThreads);
	        }
	        else
	        {
	            //DEBUGGING
	            fprintf(debuggingFile,"Sorteo: Se encontro el thread ideal el cual es %d\n", (int)headThread->threadID);
	            //DEBUGGING
	            result = headThread;
	            break;
	        }
	    }
    }
    free(generateThreads);
    return result; 
}
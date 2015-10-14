/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "RealTimeScheduler.h"
#include "SortScheduler.h"
#include "RoundRobinScheduler.h"
#include "MyThread.h"

extern TCBQueue threadsQueue;
extern sigset_t sigProcMask;

int roundRobinControl = 0;
int sortControl = 0;

static void changeScheduler();
static void checkThreads();
static void checkThread(TCB pThread);

void realTime()
{
	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
	threadsQueue->quantums++;
	checkThreads();
	changeScheduler();
}

static void checkThreads()
{
	TCB headThread = getHeadTCB(threadsQueue);
	TCB nextThread = headThread->nextThread;
    if(headThread != NULL)
    {
    	checkThread(headThread);
        while(headThread != nextThread)
        {
        	checkThread(nextThread);
            nextThread = nextThread->nextThread;
        }
    }
}

static void checkThread(TCB pThread)
{
	if(pThread->limitTime > 0 && pThread->warningLevel > 0 && pThread->threadCompleted == 0)
	{
		if(pThread->roundRobin && pThread->warningLevel == 1)
		{
			myThreadChsched(pThread);
			giveTickets(threadsQueue, pThread, WARNING_LEVEL_ONE);
			pThread->ultimateWarningLevel = pThread->warningLevel;
			pThread->warningLevel = 0;
		}
		else if(pThread->roundRobin && pThread->warningLevel > 1)
		{
			//printf("RealTime: Debe colocar primero un nivel de peligro 1 al thread %d.\n", (int)pThread->threadID);
		}
		else if(pThread->warningLevel == 1)
		{
			giveTickets(threadsQueue, pThread, WARNING_LEVEL_ONE);
			pThread->ultimateWarningLevel = pThread->warningLevel;
			pThread->warningLevel = 0;
		}
		else if(pThread->warningLevel == 2)
		{
			giveTickets(threadsQueue, pThread, WARNING_LEVEL_TWO);
			pThread->ultimateWarningLevel = pThread->warningLevel;
			pThread->warningLevel = 0;
		}
		else if(pThread->warningLevel == 3)
		{
			giveTickets(threadsQueue, pThread, WARNING_LEVEL_THREE);
			pThread->ultimateWarningLevel = pThread->warningLevel;
			pThread->warningLevel = 0;
		}
	}
}

static void changeScheduler()
{
	int sortCount = getTCBSortCount(threadsQueue);
	int roundRobinCount = getTCBRoundRobinCount(threadsQueue);
	if(sortControl &&  sortCount > 0 && roundRobinCount == 0)
	{
		//printf("RealTime: Llamando al scheduler Sort.\n");
		roundRobinControl = 0;
		sort();
	}
	else if(roundRobinControl && roundRobinCount > 0 && sortCount == 0)
	{
		//printf("RealTime: Llamando al scheduler RoundRobin.\n");
		sortControl = 0;
		roundRobin();
	}
	else if(roundRobinControl && sortCount > 0)
	{
		//printf("RealTime: Cambiando al scheduler Sort.\n");
		roundRobinControl = 0;
		sortControl = 1;
		sort();
	}
	else if(sortControl && roundRobinCount > 0)
	{
		//printf("RealTime: Cambiando al scheduler RoundRobin.\n");
		sortControl = 0;
		roundRobinControl = 1;
		roundRobin();
	}
	else
	{
		//printf("RealTime: Ningun escheduler presenta algun thread para colocar en ejecucion.\n");
		exit(0);
	}
}
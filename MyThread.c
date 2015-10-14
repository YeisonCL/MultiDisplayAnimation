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
#include "MyMutexDataStructure.h"
#include "MyThreadErrors.h"
#include "MyThread.h"


TCBQueue threadsQueue = NULL;
struct itimerval timeQuantum;
long timeInterval;
sigset_t sigProcMask;
DeadThreadsNodesQueue deadThreadsQueue = NULL;
struct sigaction schedulerHandle;
ucontext_t exitContext;
time_t randomTimeSeed;

//DEBUGGING
FILE *debuggingFile;
//DEBUGGING

extern MutexQueue mutexQueue;
extern int roundRobinControl;
extern int sortControl;

static void *wrapperFunction(void *(*pStartRoutine)(void *), void *pArgument);
static void clearBlockedThreads(TCB pTargetThread);
static void setExitContext();
static void executeExitContext();
static void setSchedulerType(TCB pThread, char *pSchedulerType);

void myThreadInit(long pTimeInterval) 
{
    //DEBUGGING
    debuggingFile = fopen("DebuggingFile.txt", "w");
    //DEBUGGING

    if (threadsQueue == NULL && deadThreadsQueue == NULL) 
    {
        sigemptyset(&sigProcMask);
        sigaddset(&sigProcMask, SIGPROF);
        deadThreadsQueue = createDeadTheadsNodesQueue();
        threadsQueue = createTCBQueue();
        if (deadThreadsQueue == NULL || threadsQueue == NULL) 
        {
            return;
        }
        else
        {
            srand((unsigned) time(&randomTimeSeed));
            timeInterval = pTimeInterval * 1000;
            threadsQueue->quantum = pTimeInterval;
            TCB TCBMain = createNewTCB();
            getcontext(&(TCBMain->threadContext));
            setExitContext();
            TCBMain->threadContext.uc_link = &exitContext;
            //La linea de abajo indica que el thread principal es administrado por el scheduler RoundRobin.
            // TCBMain->roundRobin = 1;
            // roundRobinControl = 1;
            // Descomentar las lineas comentadas de abajo y comentar la linea de arriba si se quiere que el thread principal sea administrado por el scheduler Sort.
            TCBMain->sort = 1;
            int nextTicket = searchEndTicket(threadsQueue);
            TCBMain->initialTicket = nextTicket;
            TCBMain->finalTicket = nextTicket;
            sortControl = 1;
            //
            threadsQueue->currentThread = TCBMain;
            insertThread(threadsQueue, TCBMain);
            memset(&schedulerHandle, 0, sizeof (schedulerHandle));
            schedulerHandle.sa_handler = &realTime;
            sigaction(SIGPROF, &schedulerHandle, NULL);
            //printf("\nMyThread: Biblioteca MyThread Inicializada...\n");
            timeQuantum.it_value.tv_sec = 0;
            timeQuantum.it_value.tv_usec = timeInterval;
            timeQuantum.it_interval.tv_sec = 0;
            timeQuantum.it_interval.tv_usec = timeInterval;
            setitimer(ITIMER_PROF, &timeQuantum, NULL);
        }
    }
}

int myThreadCreate(thread_t *pThread, void *(*pStartRoutine)(void *), void *pArgument, int pLimitTime, char *pSchedulerType) 
{
    if (threadsQueue != NULL) 
    {
        sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
        TCB newTCB = createNewTCB();
        getcontext(&(newTCB->threadContext));
        if (newTCB == NULL) 
        {
            freeThread(newTCB);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return NOT_ENOUGH_MEMORY;
        }
        else
        {
            if(pLimitTime > 0)
            {
                newTCB->limitTime = pLimitTime;
            }
        	newTCB->threadContext.uc_link = &exitContext;
            newTCB->startQuantum = threadsQueue->quantums;
        	setSchedulerType(newTCB, pSchedulerType);
	        makecontext(&(newTCB->threadContext), wrapperFunction, 2, pStartRoutine, pArgument);
	        *pThread = newTCB->threadID;
	        //printf("MyThread: Nuevo thread creado: %ld\n", *pThread);
	        insertThread(threadsQueue, newTCB);
	        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
	        return SUCESS;
        }
    }
    else
    {
    	return MY_THREAD_NOT_INITIALIZED;
    }
}

int myThreadJoin(thread_t pThread, void **pStatus) 
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    TCB currentThread = threadsQueue->currentThread;
    TCB targetThread = searchThread(pThread, threadsQueue);
    if (currentThread == targetThread || currentThread == NULL || (targetThread != NULL && targetThread->detach)) 
    {
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        return INVALID_OPERATION;
    }
    else
    {
        if (targetThread == NULL || targetThread->threadCompleted) 
        {
            DeadThreadNode deadThreadNode = searchDeadThread(deadThreadsQueue, pThread);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            if (deadThreadNode != NULL) 
            {
                if (pStatus != NULL) 
                {
                    *pStatus = *(deadThreadNode->returnValue);
                }
                deleteDeadThread(deadThreadsQueue, pThread);
                return SUCESS;
            } 
            else 
            {
                return INVALID_OPERATION;
            }
        }
        else
        {
            insertWaitingThread(targetThread, currentThread);
            int isBlocked = currentThread->threadBlocked;
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            while (isBlocked) 
            {
                isBlocked = currentThread->threadBlocked;
            }
            sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
            DeadThreadNode deadThreadNode = searchDeadThread(deadThreadsQueue, pThread);
            if(deadThreadNode != NULL)
            {
                if (pStatus != NULL) 
                {   
                    *pStatus = *(deadThreadNode->returnValue);
                }
                if(((deadThreadNode->threadsWaiting) - 1) == 0)
                {   
                    deleteDeadThread(deadThreadsQueue, pThread);
                }
                else
                {
                    deadThreadNode->threadsWaiting--;
                }
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                return SUCESS;
            }
            else
            {
                //printf("MyThread: Un thread anterior a este ha realizado el join primero, intente realizando el join para ambos threads antes que el thread al cual desea hacer el join haya finalizado\n");
                sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
                return SUCESS;
            }
        }
    }
}

thread_t myThreadSelf(void) 
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    TCB currentThread = threadsQueue->currentThread;
    if (currentThread == NULL) 
    {
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        return INVALID_THREAD_T;
    }
    else
    {
    	sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    	return currentThread->threadID;
    }
}

void myThreadYield(void) 
{
    raise(SIGPROF);
}

int myThreadCancel(thread_t pThread) 
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    TCB currentThread = threadsQueue->currentThread;
    if ((currentThread != NULL) && (currentThread->threadID != pThread)) 
    {
        TCB targetThread = searchThread(pThread, threadsQueue);
        if (targetThread != NULL) 
        {
            clearBlockedThreads(targetThread);
    		//printf("MyThread: Thread %ld cancelado\n", targetThread->threadID);
    		targetThread->threadCompleted = 1;
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return SUCESS;
        }
        else
        {
        	sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        	return INVALID_OPERATION;
        }
    }
    else
    {
    	sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    	return INVALID_OPERATION;
    }
}

void myThreadExit(void *pReturnValue) 
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    TCB currentTCB = threadsQueue->currentThread;
    DeadThreadNode deadThreadNode = createNewDeadThreadNode();
    if (deadThreadNode != NULL && currentTCB != NULL) 
    {
        *(deadThreadNode->returnValue) = pReturnValue;
        deadThreadNode->threadID = currentTCB->threadID;
        deadThreadNode->threadsWaiting = currentTCB->threadsWaiting;
        insertDeadThred(deadThreadsQueue, deadThreadNode);
    }
    executeExitContext();
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    raise(SIGPROF);
}

int myThreadDetach(thread_t pThread)
{
	TCB targetThread = searchThread(pThread, threadsQueue);
	if(targetThread != NULL)
	{
		targetThread->detach = 1;
		return SUCESS;
	}
	else
	{
		return INVALID_OPERATION;
	}
}

void myThreadChsched(TCB pThread)
{
    if(pThread->threadCompleted == 0)
    {
        if(pThread->sort)
        {
            sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
            pThread->sort = 0;
            pThread->roundRobin = 1;
            restructureTickets(threadsQueue, pThread);
            pThread->initialTicket = 0;
            pThread->finalTicket = 0;
            if(threadsQueue->countRoundRobin == 0)
            {
                setNewHead(threadsQueue, pThread);
            }
            threadsQueue->countRoundRobin++;
            threadsQueue->countSort--;
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        }
        else
        {
            sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
            pThread->roundRobin = 0;
            setSchedulerType(pThread, "Sort");
            moveHeadToNextRoundRobin(threadsQueue, pThread);
            threadsQueue->countRoundRobin--;
            threadsQueue->countSort++;
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        }
    }
    else
    {
        //printf("MyThread: No puede cambiar de scheduler un thread que ya ha sido completado.\n");
    }
}

void myThreadSleep(long pSleepTime)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    long actualQuantums = threadsQueue->quantums;
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    if(pSleepTime >= threadsQueue->quantum)
    {
        while(((threadsQueue->quantums - actualQuantums) * threadsQueue->quantum) <= pSleepTime);
    }
}

long myThreadGetTimeExecution(thread_t pThread)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    TCB targetThread = searchThread(pThread, threadsQueue);
    long actualQuantums = threadsQueue->quantums;
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return (actualQuantums - targetThread->startQuantum) * threadsQueue->quantum;
}

static void setSchedulerType(TCB pThread, char *pSchedulerType)
{
	if(!strcmp("Sort", pSchedulerType))
	{
		pThread->sort = 1;
        int nextTicket = searchEndTicket(threadsQueue);
        pThread->initialTicket = nextTicket;
        pThread->finalTicket = nextTicket;
	}
	else
	{
		pThread->roundRobin = 1;
	}
}

static void *wrapperFunction(void *(*pStartRoutine)(void *), void *pArgument) 
{
    void *returnValueFunction;
    TCB currentTCB = threadsQueue->currentThread;
    returnValueFunction = (*pStartRoutine)(pArgument);
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    if(!currentTCB->detach)
    {
    	DeadThreadNode deadThreadNode = createNewDeadThreadNode();
	    if (deadThreadNode != NULL) 
	    {
	        *(deadThreadNode->returnValue) = returnValueFunction;
	        deadThreadNode->threadID = currentTCB->threadID;
	        deadThreadNode->threadsWaiting = currentTCB->threadsWaiting;
	        insertDeadThred(deadThreadsQueue, deadThreadNode);
	    }
    }
    //
    //printDeadQueue(deadThreadsQueue);
    //
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return returnValueFunction;
}

static void clearBlockedThreads(TCB pTargetThread) 
{
    WaitingThreads blockedThread = pTargetThread->waitingThread;
    while(blockedThread != NULL)
    {
        blockedThread->waitingThreadNode->threadBlocked = 0;
        blockedThread = blockedThread->nextWaitingThread;
    }
}

static void executeExitContext()
{
    TCB currentThread = threadsQueue->currentThread;
    clearBlockedThreads(currentThread);
    //printf("MyThread: Thread %ld completado\n", currentThread->threadID);
    currentThread->threadCompleted = 1;
    raise(SIGPROF);
}

static void setExitContext()
{
    static int exitContextCreated;
    if(!exitContextCreated)
    {
        getcontext(&exitContext);
        exitContext.uc_link = 0;
        exitContext.uc_stack.ss_sp = malloc(STACKSIZE);
        exitContext.uc_stack.ss_size = STACKSIZE;
        exitContext.uc_stack.ss_flags= 0;
        makecontext(&exitContext, (void (*) (void))&executeExitContext, 0);
        exitContextCreated = 1;
    }
}


int myThreadMutexInit(thread_mutex_t *pMutex)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    if(pMutex != NULL)
    {
        if(!isMutexInQueue(pMutex))
        {
            MutexQueue newMutexQueue = createNewMutexQueue();
            newMutexQueue->mutex = createNewMutexNode();
            pMutex->lockNumber = newMutexQueue->mutex->lockNumber;
            insertMutexQueue(newMutexQueue);
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return SUCESS;
        }
    }
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    return INVALID_OPERATION;
}

int myThreadMutexLock(thread_mutex_t *pMutex)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    MutexQueue auxQueue = searchMutexQueue(pMutex);
    TCB currentTCB = threadsQueue->currentThread;
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    if(auxQueue == NULL)
    {        
        return INVALID_OPERATION;
    }
    else
    {
    	if(currentTCB != NULL)
	    {
            while(auxQueue->threadNodeList);
            sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
            WaitingThreads waitingTCBNode = createWaitingThreadsList();
            waitingTCBNode->waitingThreadNode = currentTCB;
            waitingTCBNode->nextWaitingThread = auxQueue->threadNodeList;
            auxQueue->threadNodeList = waitingTCBNode;
            sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
            return SUCESS;
	    }
	    else
	    {
	    	sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
	    	return INVALID_OPERATION;
	    }
    }
}

int myThreadMutexUnlock(thread_mutex_t *pMutex)
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    MutexQueue auxQueue = searchMutexQueue(pMutex);
    sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
    if(auxQueue == NULL)
    {   
        return INVALID_OPERATION;
    }
    else if(auxQueue->threadNodeList == NULL)
    {
        return INVALID_OPERATION;
    }
    else
    {
    	sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
        TCB currentTCB = threadsQueue->currentThread;
        WaitingThreads waitingTCBNode = auxQueue->threadNodeList;
        if(waitingTCBNode->waitingThreadNode == currentTCB)
        {
            auxQueue->threadNodeList = NULL;
            free(waitingTCBNode);
        }
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
	    return SUCESS;
    }
}
/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "MyThreadDataStructure.h"
#include "MyThreadErrors.h"

static long _currentThreadID = 0;

//Devuelve el threaID que sera utilizado al crear un nuevo thread.
static int getThreadID()
{
    return ++_currentThreadID;
}

TCB getHeadTCB(TCBQueue pQueue)
{
    if(pQueue == NULL)
    {
        return NULL;
    }
    else
    {
    	return pQueue->head;
    }
}

TCB getNextTCB(TCBQueue pQueue)
{
    if(pQueue != NULL && pQueue->head != NULL)
    {
        return pQueue->head->nextThread;
    }
    else
    {
    	return NULL;
    }
}

TCB createNewTCB()
{
    TCB newTCB = (TCB)malloc(sizeof(struct ThreadNodes));
    if(newTCB == NULL)
    {
    	free(newTCB);
        return NULL;
    }
    else
    {
	    newTCB->threadContext.uc_stack.ss_sp = malloc(STACKSIZE);
	    if(newTCB->threadContext.uc_stack.ss_sp == NULL)
	    {
	        free(newTCB);
	        return NULL;
	    }
	    else
	    {
	    	newTCB->threadContext.uc_stack.ss_size = STACKSIZE;
		    newTCB->threadContext.uc_stack.ss_flags = 0;
	    	newTCB->nextThread = NULL;
		    newTCB->threadContext.uc_link = 0;
		    newTCB->threadCompleted = 0;
		    newTCB->waitingThread = NULL;
		    newTCB->threadBlocked = 0;
		    newTCB->threadsWaiting = 0;
            newTCB->roundRobin = 0;
            newTCB->sort = 0;
            newTCB->initialTicket = 0;
            newTCB->finalTicket = 0;
            newTCB->warningLevel = 0;
            newTCB->ultimateWarningLevel = 0;
            newTCB->limitTime = 0;
		    newTCB->detach = 0;
            newTCB->startQuantum = 0;
		    newTCB->threadID = getThreadID();
		    return newTCB;
	    }
    }
}

TCB searchThread(thread_t pThreadID, TCBQueue pQueue)
{
    TCB headThread = getHeadTCB(pQueue);
    if(headThread == NULL)
    {
    	return NULL;
    }
    else if(headThread->threadID == pThreadID)
    {
        return headThread;
    }
    else
    {
    	TCB iteratorThread = headThread->nextThread;
	    while(headThread != iteratorThread)
	    {
	        if(iteratorThread->threadID == pThreadID)
	        {
	            return iteratorThread;
	        }
	        else
	        {
	        	iteratorThread = iteratorThread->nextThread;
	        }
	    }
	    return NULL;
    }
}

TCB searchThreadTicket(int pTicket, TCBQueue pQueue)
{
    TCB headThread = getHeadTCB(pQueue);
    if(headThread == NULL)
    {
        return NULL;
    }
    else if(headThread->initialTicket <= pTicket && headThread->finalTicket >= pTicket)
    {
        return headThread;
    }
    else
    {
        TCB iteratorThread = headThread->nextThread;
        while(headThread != iteratorThread)
        {
            if(iteratorThread->initialTicket <= pTicket && iteratorThread->finalTicket >= pTicket)
            {
                return iteratorThread;
            }
            else
            {
                iteratorThread = iteratorThread->nextThread;
            }
        }
        return NULL;  
    }
}

TCBQueue createTCBQueue()
{
    TCBQueue newQueue = (TCBQueue)malloc(sizeof(struct TCBQueues));
    if(newQueue == NULL)
    {
        return NULL;
    }
    else
    {
        newQueue->quantum = 0;
        newQueue->quantums = 0;
    	newQueue->count = 0;
        newQueue->countRoundRobin = 0;
        newQueue->countSort = 0;
	    newQueue->head = NULL;
	    newQueue->headParent = NULL;
        newQueue->currentThread = NULL;
        newQueue->currentThreadCopy = NULL;
	    return newQueue;
    }
}

WaitingThreads createWaitingThreadsList()
{
	WaitingThreads newList = (WaitingThreads)malloc(sizeof(struct WaitingThreadsList));
    if(newList != NULL)
    {
        newList->waitingThreadNode = NULL;
        newList->nextWaitingThread = NULL;
    }
    return newList;
}

DeadThreadNode searchDeadThread(DeadThreadsNodesQueue pQueue, thread_t pThreadID)
{
	if(pQueue != NULL)
    {
        DeadThreadNode deadThread = pQueue->head;
        while((deadThread != NULL) && (deadThread->threadID != pThreadID))
        {
            deadThread = deadThread->nextDeadThreadNode;
        }
        return deadThread;
    }
    else
    {
    	return NULL;
    }
}

void deleteDeadThread(DeadThreadsNodesQueue pQueue, thread_t pThreadID)
{
	DeadThreadNode deadThread = pQueue->head;
    DeadThreadNode previousDeadTread = NULL;
    while((deadThread != NULL) && (deadThread->threadID != pThreadID))
    {
        previousDeadTread = deadThread;
        deadThread = deadThread->nextDeadThreadNode;
    }
    if(deadThread != NULL)
    {
        if(previousDeadTread == NULL)
        {
            pQueue->head = deadThread->nextDeadThreadNode;
        }
        else
        {
            previousDeadTread->nextDeadThreadNode = deadThread->nextDeadThreadNode;
        }
        free(deadThread);
    }
}

DeadThreadNode createNewDeadThreadNode()
{
	DeadThreadNode newDeadThread = (DeadThreadNode)malloc(sizeof(struct DeadThreadsNodes));
    if(newDeadThread == NULL)
    {
        return NULL;
    }
    else
    {
    	newDeadThread->returnValue = (void**)malloc(sizeof(void*));
	    if(newDeadThread->returnValue == NULL)
	    {
	        free(newDeadThread);
	        return NULL;
	    }
	    else
	    {
	    	newDeadThread->threadID = INVALID_THREAD_T;
		    *(newDeadThread->returnValue) = NULL;
		    newDeadThread->threadsWaiting = 0;
		    newDeadThread->nextDeadThreadNode = NULL;
		    return newDeadThread;
	    }
    }
}

DeadThreadsNodesQueue createDeadTheadsNodesQueue()
{
	DeadThreadsNodesQueue newQueue = (DeadThreadsNodesQueue)malloc(sizeof(struct DeadThreadNodeQueue));
    if(newQueue == NULL)
    {
        return NULL;
    }
    newQueue->count = 0;
    newQueue->head = NULL;
    return newQueue;
}

int moveForward(TCBQueue pQueue)
{
	int result = INVALID_OPERATION;
    if(pQueue != NULL)
    {
        TCB headThread = pQueue->head;
        if(headThread != NULL)
        {
        	pQueue->head = headThread->nextThread;
            pQueue->headParent = headThread;
            result = SUCESS;
        }
    }
    return result;
}

int getTCBCount(TCBQueue pQueue)
{
	if(pQueue == NULL)
    {
        return 0;
    }
    else
    {
    	return pQueue->count;
    }
}

int getTCBRoundRobinCount(TCBQueue pQueue)
{
    if(pQueue == NULL)
    {
        return 0;
    }
    else
    {
        return pQueue->countRoundRobin;
    }
}

int getTCBSortCount(TCBQueue pQueue)
{
    if(pQueue == NULL)
    {
        return 0;
    }
    else
    {
        return pQueue->countSort;
    }
}

void freeThread(TCB pThread)
{
    if(pThread != NULL)
    {
        WaitingThreads waitingThreadNode = pThread->waitingThread;
        free(pThread->threadContext.uc_stack.ss_sp);
        while(waitingThreadNode != NULL)
        {
            WaitingThreads nextWaitingThreadNode = waitingThreadNode->nextWaitingThread;
            free(waitingThreadNode);
            waitingThreadNode = nextWaitingThreadNode;
        }
        free(pThread);
    }
}

int deleteHeadThread(TCBQueue pQueue)
{
	int result = SUCESS;
    TCB previousThread, headThread;
    if(pQueue == NULL)
    {
        result = INVALID_OPERATION;
    }
    else
    {
        headThread = pQueue->head;
        previousThread = pQueue->headParent;
        if(headThread != NULL)
        {
            if(pQueue->count == 1)
            {
                pQueue->head = NULL;
                pQueue->headParent = NULL;
            }
            else
            {
                pQueue->head = headThread->nextThread;
                previousThread->nextThread = pQueue->head;
            }
            headThread->roundRobin == 1 ? pQueue->countRoundRobin-- : pQueue->countSort--;
            freeThread(headThread);
            pQueue->count--;
        }
        else
        {
            result = INVALID_OPERATION;
        }
    }
    return result;
}

int deleteSortThread(thread_t pThreadID, TCBQueue pQueue)
{
    int result = SUCESS;
    TCB headThread;
    if(pQueue == NULL)
    {
        result = INVALID_OPERATION;
    }
    else
    {
        headThread = pQueue->head;
        if(headThread != NULL)
        {
            if(pQueue->count == 1)
            {
                pQueue->head = NULL;
                pQueue->headParent = NULL;
            }
            else if(headThread->threadID == pThreadID)
            {
                pQueue->headParent->nextThread = headThread->nextThread;
                pQueue->head = pQueue->headParent->nextThread;
                if((pQueue->countSort - 1) != 0)
                {
                    restructureTickets(pQueue, headThread);
                }
                free(headThread);
            }
            else
            {
                while(headThread->nextThread->threadID != pThreadID)
                {
                    headThread = headThread->nextThread;
                }
                TCB auxThred = headThread->nextThread;
                headThread->nextThread = auxThred->nextThread;
                if(headThread->nextThread == pQueue->head)
                {
                    pQueue->headParent = headThread;
                }
                if((pQueue->countSort - 1) != 0)
                {
                    restructureTickets(pQueue, auxThred);
                }
                free(auxThred);
            }
            pQueue->count--;
            pQueue->countSort--;
        }
        else
        {
            result = INVALID_OPERATION;
        }
    }
    return result;
}

int insertThread(TCBQueue pQueue, TCB pThread)
{
	int result = SUCESS;
    if(pQueue == NULL || pThread == NULL)
    {
        result = INVALID_OPERATION;
    }
    else
    {
        if(pQueue->head == NULL)
        {
            pThread->nextThread = pThread;
            pQueue->headParent = pThread;
            pQueue->head = pThread;
        }
        else if(pQueue->countRoundRobin == 0)
        {
            pThread->nextThread = pQueue->head;
            pQueue->headParent->nextThread = pThread;
            pQueue->head = pThread;
        }
        else
        {
            pThread->nextThread = pQueue->head;
            pQueue->headParent->nextThread = pThread;
            pQueue->headParent = pThread;
        }
        pQueue->count++;
        pThread->roundRobin == 1 ? pQueue->countRoundRobin++ : pQueue->countSort++;
    }
    return result;
}

int insertWaitingThread(TCB pTargetThread, TCB pThreadAdded)
{
	WaitingThreads newWaitingThread = createWaitingThreadsList();
    if(newWaitingThread != NULL)
    {
    	pTargetThread->threadsWaiting++;
        newWaitingThread->waitingThreadNode = pThreadAdded;
        newWaitingThread->nextWaitingThread = pTargetThread->waitingThread;
        pTargetThread->waitingThread = newWaitingThread;
        pThreadAdded->threadBlocked = 1;
        return SUCESS;
    }
    else
    {
    	return INVALID_OPERATION;
    }
}

int insertDeadThred(DeadThreadsNodesQueue pQueue, DeadThreadNode pThread)
{
    if(pQueue != NULL && pThread != NULL)
    {
        pThread->nextDeadThreadNode = pQueue->head;
        pQueue->head = pThread;
        return SUCESS;
    }
    else
    {
    	return INVALID_OPERATION;
    }
}

int searchEndTicket(TCBQueue pQueue)
{
    TCB headThread = getHeadTCB(pQueue);
    if(headThread == NULL)
    {
        return 1;
    }
    else
    {
        int nextTicket = 0;
        TCB nextThread = headThread->nextThread;
        if(headThread->sort && headThread->finalTicket > nextTicket)
        {
            nextTicket = headThread->finalTicket;
        }
        while(nextThread != headThread)
        {
            if(nextThread->sort)
            {
                if(nextThread->finalTicket > nextTicket)
                {
                    nextTicket = nextThread->finalTicket;
                }
                nextThread = nextThread->nextThread;
            }
            else
            {
                nextThread = nextThread->nextThread;
            }
        }
        return ++nextTicket;
    }
}

void giveTickets(TCBQueue pQueue, TCB pThread, int pTickets)
{
    TCB headThread = getHeadTCB(pQueue);
    if(headThread != NULL)
    {
        TCB nextThread = headThread->nextThread;
        int previousTicket = pThread->finalTicket;
        pThread->finalTicket = pThread->finalTicket + pTickets;
        if(headThread->sort && headThread != pThread && headThread->initialTicket > previousTicket)
        {
            headThread->initialTicket = headThread->initialTicket + pTickets;
            headThread->finalTicket = headThread->finalTicket + pTickets;
        }
        while(nextThread != headThread)
        {
            if(nextThread->sort && nextThread != pThread)
            {
                if(nextThread->initialTicket > previousTicket)
                {
                    nextThread->initialTicket = nextThread->initialTicket + pTickets;
                    nextThread->finalTicket = nextThread->finalTicket + pTickets;
                }
                nextThread = nextThread->nextThread;
            }
            else
            {
                nextThread = nextThread->nextThread;
            }
        }
    }
}

void deleteTickets(TCBQueue pQueue, TCB pThread, int pTickets)
{
    TCB headThread = getHeadTCB(pQueue);
    if(headThread != NULL)
    {
        TCB nextThread = headThread->nextThread;
        int previousTicket = pThread->finalTicket;
        if(pTickets > (pThread->finalTicket - pThread->initialTicket))
        {
            pTickets = pThread->finalTicket - pThread->initialTicket;
            pThread->finalTicket = pThread->initialTicket;
        }
        else
        {
            pThread->finalTicket = pThread->finalTicket - pTickets;
        }
        if(headThread->sort && headThread != pThread && headThread->initialTicket > previousTicket)
        {
            headThread->initialTicket = headThread->initialTicket - pTickets;
            headThread->finalTicket = headThread->finalTicket - pTickets;
        }
        while(nextThread != headThread)
        {
            if(nextThread->sort && nextThread != pThread)
            {
                if(nextThread->initialTicket > previousTicket)
                {
                    nextThread->initialTicket = nextThread->initialTicket - pTickets;
                    nextThread->finalTicket = nextThread->finalTicket - pTickets;
                }
                nextThread = nextThread->nextThread;
            }
            else
            {
                nextThread = nextThread->nextThread;
            }
        }
    }
}

void moveHeadToNextRoundRobin(TCBQueue pQueue, TCB pThread)
{
    if(getTCBRoundRobinCount(pQueue) > 1 && getHeadTCB(pQueue) == pThread)
    {
        TCB auxThred = pThread->nextThread;
        while(auxThred->sort)
        {
            auxThred = auxThred->nextThread;
        }
        setNewHead(pQueue, auxThred);
    }
}

void setNewHead(TCBQueue pQueue, TCB pThread)
{
    TCB headThread = getHeadTCB(pQueue);
    while(headThread != pThread)
    {
        moveForward(pQueue);
        headThread = getHeadTCB(pQueue);
    }
}

void restructureTickets(TCBQueue pQueue, TCB pErasedThread)
{
    TCB headThread = getHeadTCB(pQueue);
    TCB nextThread = headThread->nextThread;
    int erasedTickets = (pErasedThread->finalTicket - pErasedThread->initialTicket) + 1;
    if(headThread != NULL)
    {
        if(headThread->sort && headThread->initialTicket > pErasedThread->finalTicket)
        {
            headThread->initialTicket = headThread->initialTicket - erasedTickets;
            headThread->finalTicket = headThread->finalTicket - erasedTickets;
        }
        while(nextThread != headThread)
        {
            if(nextThread->sort && nextThread->initialTicket > pErasedThread->finalTicket)
            {
                nextThread->initialTicket = nextThread->initialTicket - erasedTickets;
                nextThread->finalTicket = nextThread->finalTicket - erasedTickets;
                nextThread = nextThread->nextThread;
            }
            else
            {
                nextThread = nextThread->nextThread;
            }
        }
    }
}

void printTickets(TCBQueue pQueue)
{
    TCB headThread = getHeadTCB(pQueue);
    TCB nextThread = headThread->nextThread;
    if(headThread->sort)
    {
        printf("Soy el thread %d con el tiquete inicial %d y el tiquete final %d\n", (int)headThread->threadID, headThread->initialTicket, headThread->finalTicket);
    }
    while(headThread != nextThread)
    {
        if(nextThread->sort)
        {
            printf("Soy el thread %d con el tiquete inicial %d y el tiquete final %d\n", (int)nextThread->threadID, nextThread->initialTicket, nextThread->finalTicket);
            nextThread = nextThread->nextThread;
        }
        else
        {
            nextThread = nextThread->nextThread;
        }
    }
}

void printQueue(TCBQueue pQueue)
{
    TCB headThread = getHeadTCB(pQueue);
    TCB nextThread = headThread->nextThread;
    if(headThread != NULL)
    {
        printf("Imprimiendo cola\n");
        printf("Head: %d\n", (int)headThread->threadID);
        printf("Parent: %d\n", (int)pQueue->headParent->threadID);
        printf("Thread: %d\n", (int)headThread->threadID);
        while(headThread != nextThread)
        {
            printf("Thread: %d\n", (int)nextThread->threadID);
            nextThread = nextThread->nextThread;
        }
        printf("Termino la impresion de la cola\n");
    }
}

void printDeadQueue(DeadThreadsNodesQueue pQueue)
{
    DeadThreadNode deadNode = pQueue->head;
    printf("Imprimiendo cola de threads muertos\n");
    while(deadNode != NULL)
    {
        printf("Thead: %d\n", (int)deadNode->threadID);
        deadNode = deadNode->nextDeadThreadNode;
    }
    printf("Termino impresion cola de threads muertos\n");
}
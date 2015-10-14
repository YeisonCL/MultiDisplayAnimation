/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "MyMutexDataStructure.h"
#include "MyThreadErrors.h"

MutexQueue mutexQueue = NULL;

int isMutexInQueue(thread_mutex_t *pMutex)
{
    if(mutexQueue == NULL || pMutex == NULL)
    {
        return 0;
    }
    else
    {
    	MutexQueue auxQueue = mutexQueue;
	    while((auxQueue != NULL)  && (auxQueue->mutex->lockNumber != pMutex->lockNumber))
	    {
	        auxQueue = auxQueue->nextMutex;
	    }
	    if(auxQueue == NULL)
	    {
	    	return 0;
	    }
	    else
	    {
	    	return 1;
	    }
    }
}

MutexQueue createNewMutexQueue()
{
    MutexQueue auxQueue = (MutexQueue)malloc(sizeof(struct MutexQueueNodes));
    if(auxQueue != NULL)
    {
        auxQueue->mutex = NULL;
        auxQueue->nextMutex = NULL;
        auxQueue->threadNodeList = NULL;
        return auxQueue;
    }
    else
    {
    	return NULL;
    }
}

thread_mutex_t* createNewMutexNode()
{
    static long currentMutexValue = 0;
    thread_mutex_t *newMutexNode = (thread_mutex_t*)malloc(sizeof(struct MutexNode));
    newMutexNode->lockNumber = ++currentMutexValue;
    return newMutexNode;
}

void insertMutexQueue(MutexQueue pMutex)
{
    if(pMutex != NULL)
    {
        pMutex->nextMutex = mutexQueue;
        mutexQueue = pMutex;
    }
}

MutexQueue searchMutexQueue(thread_mutex_t *pMutex)
{
    if(mutexQueue == NULL || pMutex == NULL)
    {
        return NULL;
    }
    else
    {
    	MutexQueue auxQueue = mutexQueue;
	    while((auxQueue != NULL)  && (auxQueue->mutex->lockNumber != pMutex->lockNumber))
	    {
	        auxQueue = auxQueue->nextMutex;
	    }
	    return auxQueue;
    }
}
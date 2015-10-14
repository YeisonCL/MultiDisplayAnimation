/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _MYMUTEXDATASTRUCTURE_H
#define _MYMUTEXDATASTRUCTURE_H

#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "MyThreadDataStructure.h"
    
typedef struct MutexNode
{
   long lockNumber;
} thread_mutex_t;

typedef struct MutexQueueNodes
{
    thread_mutex_t *mutex;
    struct MutexQueueNodes *nextMutex;
    WaitingThreads threadNodeList;
} *MutexQueue;

MutexQueue searchMutexQueue(thread_mutex_t *pMutex);
MutexQueue createNewMutexQueue();
thread_mutex_t *createNewMutexNode();
void insertMutexQueue(MutexQueue pMutex);
int isMutexInQueue(thread_mutex_t *pMutex);

#endif
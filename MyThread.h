/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _MYTHREAD_H
#define	_MYTHREAD_H

#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "MyThreadDataStructure.h"
#include "MyMutexDataStructure.h"

//FUNCIONES DE THREADS

void myThreadInit(long pTimeInterval);
int myThreadCreate(thread_t *pThread, void *(*pStartRoutine)(void *), void *pArgument, int pLimitTime, char *pSchedulerType);
thread_t myThreadSelf(void);
void myThreadYield(void);
void myThreadExit(void *pReturnValue);
void myThreadChsched(TCB pThread);
void myThreadSleep(long pSleepTime);
int myThreadCancel(thread_t pThread);
int myThreadJoin(thread_t pThread, void **pStatus);
int myThreadDetach(thread_t pThread);
long myThreadGetTimeExecution(thread_t pThread);

//FUNCIONES DE MUTEX

int myThreadMutexInit(thread_mutex_t *pMutex);
int myThreadMutexLock(thread_mutex_t *pMutex);
int myThreadMutexUnlock(thread_mutex_t *pMutex);

#endif
/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "RoundRobinScheduler.h"
#include "MyThread.h"

extern TCBQueue threadsQueue;
extern struct itimerval timeQuantum;
extern long timeInterval;
extern sigset_t sigProcMask;

//DEBUGGING
extern FILE *debuggingFile;
//DEBUGGING

static TCB getRoundRobinHead(TCBQueue pQueue);

void roundRobin()
{
    sigprocmask(SIG_BLOCK, &sigProcMask, NULL);
    TCB currentThread = getRoundRobinHead(threadsQueue);
    TCB previousThread = threadsQueue->currentThread;
    if(previousThread == NULL && currentThread == NULL)
    {
        //Deadlock
        fprintf(debuggingFile,"RoundRobin: Se ha presentado un error no controlado, posible DEADLOCK entre los hilos creados.\n");   
        fflush(debuggingFile);
        exit(1);
    }
    else if(currentThread == NULL)
    {
        //DEBUGGING
        fprintf(debuggingFile,"RoundRobin: currentThread presenta un valor de NULL\n");      
        fflush(debuggingFile);
        //DEBUGGING
        //Todos los threads existentes administrados por el scheduler tipo RoundRobin estan bloqueados, probablemente a la espera de que se desbloque uno de tipo Sorteo o bien 
        //ya no existen threads administrados por RoundRobin para colocar.
        threadsQueue->currentThreadCopy = threadsQueue->currentThread;
        threadsQueue->currentThread = NULL;
        sigprocmask(SIG_UNBLOCK, &sigProcMask, NULL);
        myThreadYield();
    }
    else if(previousThread == NULL && currentThread != threadsQueue->currentThreadCopy)
    {
        //Es posible que el scheduler tipo Sorteo se quedara sin threads para colocar o tenga todos bloqueados a la espera que se libere uno de tipo RoundRobin, 
        //o bien que el thread anterior tambien fuese de tipo RoundRobin y ya estuviera completo.
        threadsQueue->currentThread = currentThread;
        timeQuantum.it_value.tv_usec = timeInterval;
        if(threadsQueue->currentThreadCopy != NULL)
        {
            //DEBUGGING
            fprintf(debuggingFile, "RoundRobin: previousThread presenta un valor de NULL, haciendo el cambio de contexto, remplazando thread %d por thread %d\n", (int)threadsQueue->currentThreadCopy->threadID, (int)currentThread->threadID );
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
            fprintf(debuggingFile,"RoundRobin: previousThread presenta un valor de NULL, colocando en contexto el thread %d\n", (int)currentThread->threadID);
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
            fprintf(debuggingFile,"RoundRobin: Haciendo el cambio de contexto, remplazando thread %d por thread %d\n", (int)previousThread->threadID, (int)currentThread->threadID);
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

//DEBE DAR UN THREAD QUE SEA VALIDO, ES DECIR, QUE NO ESTE BLOQUEADO O TERMINADO O SEA DE TIPO SORTEO, EN CASO DE QUE NO HAYA ALGUNO DEBE RETORNAR NULL,
//ADEMAS TAMBIEN DEBE BORRAR LOS THREADS QUE YA ESTEN COMPLETOS.
static TCB getRoundRobinHead(TCBQueue pQueue)
{
    TCB result = NULL;
    int count = 0;
    //DEBUGGING
    fprintf(debuggingFile,"RoundRobin: La cabeza actual previo al while es el thread %d\n", (int)getHeadTCB(pQueue)->threadID);
    //DEBUGGING
    if(getHeadTCB(pQueue)->threadCompleted == 0)
    {
        moveForward(pQueue);
    }
    //DEBUGGING
    fprintf(debuggingFile,"RoundRobin: Despues del movimiento previo al while la cabeza es el thread %d\n", (int)getHeadTCB(pQueue)->threadID);
    //DEBUGGING
    TCB headThread = getHeadTCB(pQueue);
    while(count < getTCBCount(pQueue))
    {
        //DEBUGGING
        fprintf(debuggingFile,"RoundRobin: Dentro del while la cabeza es el thread %d\n", (int)headThread->threadID);
        //DEBUGGING
        if(getTCBRoundRobinCount(pQueue) == 0)
        {
            //DEBUGGING
            fprintf(debuggingFile,"RoundRobin: Se han acabado los threads que administra el scheduler RoundRobin en la cola\n");
            //DEBUGGING
            result = NULL;
            break;
        }
        else if(headThread->threadCompleted && headThread->roundRobin)
        {
            //DEBUGGING
            fprintf(debuggingFile,"RoundRobin: Thread numero %d completado por ende se va a eliminar\n", (int)headThread->threadID);
            //DEBUGGING
            if(pQueue->currentThread == headThread)
            {
                pQueue->currentThread = NULL;
            }
            deleteHeadThread(pQueue);
            headThread = getHeadTCB(pQueue);
        }
        else if(headThread->threadBlocked || headThread->sort)
        {
            //DEBUGGING
            fprintf(debuggingFile,"RoundRobin:  Thread %d bloqueado o es administrado por el scheduler tipo Sorteo\n", (int)headThread->threadID);
            //DEBUGGING
            moveForward(pQueue);
            headThread = getHeadTCB(pQueue);
            count++;
        }
        else
        {
            //DEBUGGING
            fprintf(debuggingFile,"RoundRobin: Se encontro el thread ideal el cual es %d\n", (int)headThread->threadID);
            //DEBUGGING
            result = headThread;
            break;
        }
    }
    return result; 
}
/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _MYTHREADDATASTRUCTURE_H
#define _MYTHREADDATASTRUCTURE_H

#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define STACKSIZE 1024*8

typedef long thread_t;

typedef struct ThreadNodes
{
    thread_t threadID;
    ucontext_t threadContext;
    struct ThreadNodes *nextThread;
    struct WaitingThreadsList *waitingThread;
    long startQuantum;
    int threadCompleted;
    int threadBlocked;
    int threadsWaiting;
    int roundRobin;
    int sort;
    int initialTicket;
    int finalTicket;
    int warningLevel;
    int ultimateWarningLevel;
    int limitTime;
    int detach;
}*TCB;

typedef struct WaitingThreadsList
{
    TCB waitingThreadNode;
    struct WaitingThreadsList *nextWaitingThread;
}*WaitingThreads;

typedef struct TCBQueues
{
    TCB head;
    TCB headParent;
    TCB currentThread;
    TCB currentThreadCopy;
    long countRoundRobin;
    long countSort;
    long count;
    long quantum;
    long quantums;
}*TCBQueue;

typedef struct DeadThreadsNodes
{
    thread_t threadID;
    void **returnValue;
    int threadsWaiting;
    struct DeadThreadsNodes *nextDeadThreadNode;
}*DeadThreadNode;

typedef struct DeadThreadNodeQueue
{
    DeadThreadNode head;
    long count;
}*DeadThreadsNodesQueue;

TCB getHeadTCB(TCBQueue pQueue); //Obtiene el thread principal de la cola, el thread que se encuentra en la cabeza de la cola.
TCB getNextTCB(TCBQueue pQueue); //Obtiene el thread que le sigue a la cabeza.
TCB createNewTCB(); //Crea un nuevo nodo para un thread.
TCB searchThread(thread_t pThreadID,TCBQueue pQueue); //Retorna un thread buscando por su ID en la cola.
TCB searchThreadTicket(int pTicket, TCBQueue pQueue); //Retorna un thread buscando por su ID de tickete.
TCBQueue createTCBQueue(); //Instancia una nueva cola donde se guardaran los nodos que contienen los threads.
WaitingThreads createWaitingThreadsList(); //Inicializa una nueva lista donde se guardaran los threds en espera de que otro finalice.
DeadThreadNode searchDeadThread(DeadThreadsNodesQueue pQueue, thread_t pThreadID); //Obtiene un thread dado su threadID de la lista de threads muertos.
DeadThreadNode createNewDeadThreadNode(); //Instancia un nuevo nodo donde se guardara un thread que ya haya finalizado.
DeadThreadsNodesQueue createDeadTheadsNodesQueue(); //Instancia una nueva cola donde se guardaran los threads que ya han terminado.
int moveForward(TCBQueue pQueue); //Avanza en la estructura, es decir, cambia la cabeza al siguiente y pone en el padre de la cabeza el thread anterior a la cabeza actual en la cola.
int getTCBCount(TCBQueue pQueue); //Retorna la cantidad de threads que hay en la cola.
int getTCBRoundRobinCount(TCBQueue pQueue); //Retorna la cantidad de threads adaministrados por el scheduler RoundRobin que hay en la cola.
int getTCBSortCount(TCBQueue pQueue); //Retorna la cantidad de threads administrador por el scheduler Sort que hay en la cola.
int deleteHeadThread(TCBQueue pQueue); //Elimina el thread cabeza en la cola.
int deleteSortThread(thread_t pThreadID, TCBQueue pQueue); //Elimina el thread dado de la cola.
int insertThread(TCBQueue pQueue, TCB pThread); //Agrega un nuevo thread al final de la cola (Que a su vez es una lista circular).
int insertWaitingThread(TCB pTargetThread,TCB pThreadAdded); //Agrega un nuevo thread a la lista de threads en espera (threads que hicieron un join para esperar la finalizacion de otro thread).
int insertDeadThred(DeadThreadsNodesQueue pQueue, DeadThreadNode pThread);// Agrega un nuevo thread al inicio de la cola y setea la caebeza al nuevo thread agregado.
int searchEndTicket(TCBQueue pQueue); //Busca el ultimo ticket que fue asignado a un proceso y lo devuelve sumandole uno.
void deleteDeadThread(DeadThreadsNodesQueue pQueue, thread_t pThreadID); //Borra un deadThread de la lista.
void freeThread(TCB pThread); //Libera el espacio asociado a un thread en la cola.
void giveTickets(TCBQueue pQueue, TCB pThread, int pTickets); //Da mas ticketes al thread dado con la cantidad de tiquetes dados.
void deleteTickets(TCBQueue pQueue, TCB pThread, int pTickets); //Borra ticketes al thread dado la cantidad enviada.
void setNewHead(TCBQueue pQueue, TCB pThread); //Setea la cabeza al nuevo thread dado.
void restructureTickets(TCBQueue pQueue, TCB pErasedThread); //Reestructura por completo los ticketes de los threads administrados por el scheduler sort.
void moveHeadToNextRoundRobin(TCBQueue pQueue, TCB pThread); //Mueve la cabeza al siguiente thread administrado por un scheduler RoundRobin.

//DEBUGGING
void printTickets(TCBQueue pQueue);
void printQueue(TCBQueue pQueue);
void printDeadQueue(DeadThreadsNodesQueue pQueue);
//DEBUGGING

#endif
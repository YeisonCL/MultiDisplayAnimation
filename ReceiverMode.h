/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/


#ifndef _RECEIVERMODE_H
#define _RECEIVERMODE_H

#include <stdio.h>     
#include <stdlib.h>     
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#define MAX_CONNECTIONS 2048

void startReceiverMode(int pPort);

#endif
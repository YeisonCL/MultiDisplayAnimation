/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _TRANSMITTERMODE_H
#define _TRANSMITTERMODE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <errno.h>
#include <unistd.h>

void startTransmitterMode(int pConnectionsCounter);
void sendFigure(char *pReadBuffer, int pNumberCharacters, int pConnection);
void freeMemoryTransmitterMode(int pConnectionsCounter);

#endif
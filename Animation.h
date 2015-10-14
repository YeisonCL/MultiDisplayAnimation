/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <ncurses.h>

#include "MyThread.h"

typedef struct ASCIIArt
{
    int limitTime[2]; //inicial:final
    int canvasSize[2]; //width:height
    char *objectName; //ejem: figura1
    char *figureType; //ejemplo:RoundRobin
    char ***explosion; //Figura de explosion;
    char **object; //Contiene la figura ASCII actual.
    char **object0; //Contiene la figura ASCII a 0 grados.
    char **object90; //Contiene la figura ASCII a 90 grados.
    char **object180; //Contiene la figura ASCII a 180 grados.
    char **object270; //Contiene la figura ASCII a 270 grados.
    int objectSize[2]; //currentWidth:currentHeight
    int objectSize0[2]; //0Width:0Height
    int objectSize90[2]; //90Width:90Height
    int objectSize180[2]; //180Width:180Height
    int objectSize270[2]; //270Width:270Height
    int position[4]; //XInitialPosition:YInitialPosition-XFinalPosition:YFinalPosition
    int currentPosition[2]; //currentPositionX:currentPositionY
    int angle[2]; //InitialAngle:FinalAngle
    int currentAngle; //currentAngle;
    int endAnimation; //endAnimation
    thread_t threadID; //threadID
}*ASCIIFigure;

void createArrayAnimation(int pFigureCounter);
void insertFigureToAnimation(ASCIIFigure pFigure);
void startAnimation();

#endif
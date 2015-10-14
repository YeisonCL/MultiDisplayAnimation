/*********************************************/
/*    Instituto Tecnológico de Costa Rica  */
/*    Ingeniería en Computadores           */
/*    Principios de Sistemas Operativos    */
/*    II Semestre 2015                     */
/*                                         */
/*    Author: Yeison Arturo Cruz León      */
/*    Carné: 201258348                     */
/*********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "Animation.h"
#include "PrincipalMenu.h"

//DEBUGGING
extern FILE *debuggingFile;
//DEBUGGING

int main(int argc, char** argv) 
{
	initscr();
	curs_set(0);
	principalMenu();
	endwin();
	fclose(debuggingFile);
}
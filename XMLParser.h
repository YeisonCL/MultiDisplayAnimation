/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _XMLPARSER_H
#define _XMLPARSER_H

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "Animation.h"

xmlDocPtr createDocumentXML(char *pDocumentName);
void insertNewFigure(ASCIIFigure pFigure, xmlDocPtr pDocument);
void insertNewConnection(char *pIp, int pPort, xmlDocPtr pDocument);
ASCIIFigure extractFigure(xmlDocPtr pDocument, char pIndexFigure);
int getCounter(xmlDocPtr pDocument);
char *extractIP(xmlDocPtr pDocument, char *pIndexConnection);
int extractPort(xmlDocPtr pDocument, char *pIndexConnection);
void modifyFigureXML(int pIndexFigure, char *pFeature, int pNewValue, xmlDocPtr pDocument);
void insertNewSettings(int pQuantum, int pFiguresDelay, int pExplosionVelocity, xmlDocPtr pDocument);
int extractSettings(xmlDocPtr pDocument, int pIndexSetting);

#endif
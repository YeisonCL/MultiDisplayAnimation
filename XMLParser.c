/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "XMLParser.h"

xmlDocPtr createDocumentXML(char *pDocumentName)
{
	struct stat dataFile;
	xmlDocPtr xmlDocument;
	if(lstat(pDocumentName, &dataFile) == -1)
	{
		xmlDocument = xmlNewDoc(BAD_CAST "1.0");
		xmlNodePtr root, counter;
	   	root = xmlNewNode(NULL, BAD_CAST "Root");
		counter = xmlNewChild(root, NULL, BAD_CAST "Contador", NULL);
		xmlNodeSetContent(counter, xmlEncodeSpecialChars(xmlDocument, BAD_CAST "0"));
		xmlDocSetRootElement(xmlDocument, root);
	}
	else
	{
		xmlDocument = xmlReadFile(pDocumentName, NULL, 0);
	}
	return xmlDocument;
}

void insertNewFigure(ASCIIFigure pFigure, xmlDocPtr pDocument)
{
	xmlNodePtr currentRoot, counter, root, initialLimitTime, finalLimitTime, widthCanvasSize, heightCanvasSize, XInitialPosition, YInitialPosition, XFinalPosition, YFinalPosition, initialAngle, finalAngle, figureType;
    xmlNodePtr widthFigure0, heightFigure0, widthFigure90, heightFigure90, widthFigure180, heightFigure180, widthFigure270, heightFigure270;
	char *value;
	int newValue;
	char* stringCharacter = (char*)malloc(16 * sizeof(char));
	root = xmlNewNode(NULL, BAD_CAST pFigure->objectName);
	figureType = xmlNewChild(root, NULL, BAD_CAST "TipoScheduler", NULL);
	initialLimitTime = xmlNewChild(root, NULL, BAD_CAST "TiempoLimiteInicial", NULL);
	finalLimitTime = xmlNewChild(root, NULL, BAD_CAST "TiempoLimiteFinal", NULL);
	widthCanvasSize = xmlNewChild(root, NULL, BAD_CAST "AnchoCanvas", NULL);
	heightCanvasSize = xmlNewChild(root, NULL, BAD_CAST "AlturaCanvas", NULL);
	widthFigure0 = xmlNewChild(root, NULL, BAD_CAST "AnchoFigura0", NULL);
	heightFigure0 = xmlNewChild(root, NULL, BAD_CAST "AlturaFigura0", NULL);
    widthFigure90 = xmlNewChild(root, NULL, BAD_CAST "AnchoFigura90", NULL);
    heightFigure90 = xmlNewChild(root, NULL, BAD_CAST "AlturaFigura90", NULL);
    widthFigure180 = xmlNewChild(root, NULL, BAD_CAST "AnchoFigura180", NULL);
    heightFigure180 = xmlNewChild(root, NULL, BAD_CAST "AlturaFigura180", NULL);
    widthFigure270 = xmlNewChild(root, NULL, BAD_CAST "AnchoFigura270", NULL);
    heightFigure270 = xmlNewChild(root, NULL, BAD_CAST "AlturaFigura270", NULL);
	XInitialPosition = xmlNewChild(root, NULL, BAD_CAST "PosicionInicialX", NULL);
	YInitialPosition = xmlNewChild(root, NULL, BAD_CAST "PosicionInicialY", NULL);
	XFinalPosition = xmlNewChild(root, NULL, BAD_CAST "PosicionFinalX", NULL);
	YFinalPosition = xmlNewChild(root, NULL, BAD_CAST "PosicionFinalY", NULL);
	initialAngle = xmlNewChild(root, NULL, BAD_CAST "AnguloInicial", NULL);
	finalAngle = xmlNewChild(root, NULL, BAD_CAST "AnguloFinal", NULL);
	xmlNodeSetContent(figureType, xmlEncodeSpecialChars(pDocument, BAD_CAST pFigure->figureType));
	snprintf(stringCharacter, 16, "%d", pFigure->limitTime[0]);
	xmlNodeSetContent(initialLimitTime, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->limitTime[1]);
	xmlNodeSetContent(finalLimitTime, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->canvasSize[0]);
	xmlNodeSetContent(widthCanvasSize, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->canvasSize[1]);
	xmlNodeSetContent(heightCanvasSize, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->objectSize0[0]);
	xmlNodeSetContent(widthFigure0, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->objectSize0[1]);
	xmlNodeSetContent(heightFigure0, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    snprintf(stringCharacter, 16, "%d", pFigure->objectSize90[0]);
    xmlNodeSetContent(widthFigure90, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    snprintf(stringCharacter, 16, "%d", pFigure->objectSize90[1]);
    xmlNodeSetContent(heightFigure90, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    snprintf(stringCharacter, 16, "%d", pFigure->objectSize180[0]);
    xmlNodeSetContent(widthFigure180, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    snprintf(stringCharacter, 16, "%d", pFigure->objectSize180[1]);
    xmlNodeSetContent(heightFigure180, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    snprintf(stringCharacter, 16, "%d", pFigure->objectSize270[0]);
    xmlNodeSetContent(widthFigure270, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    snprintf(stringCharacter, 16, "%d", pFigure->objectSize270[1]);
    xmlNodeSetContent(heightFigure270, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->position[0]);
	xmlNodeSetContent(XInitialPosition, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->position[1]);
	xmlNodeSetContent(YInitialPosition, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->position[2]);
	xmlNodeSetContent(XFinalPosition, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->position[3]);
	xmlNodeSetContent(YFinalPosition, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->angle[0]);
	xmlNodeSetContent(initialAngle, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	snprintf(stringCharacter, 16, "%d", pFigure->angle[1]);
	xmlNodeSetContent(finalAngle, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	currentRoot = xmlDocGetRootElement(pDocument);
	counter = currentRoot->children;
	value = (char*)xmlNodeGetContent(counter);
	newValue = atoi(value) + 1;
	snprintf(stringCharacter, 16, "%d", newValue);
	xmlNodeSetContent(counter, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
	xmlAddChild(currentRoot, root);
	xmlSaveFileEnc("figuras.xml", pDocument, "UTF-8");
	free(stringCharacter);
	free(pFigure->objectName);
	free(pFigure->figureType);
	free(pFigure);
	xmlFreeDoc(pDocument);
	xmlCleanupParser();
}

void modifyFigureXML(int pIndexFigure, char *pFeature, int pNewValue, xmlDocPtr pDocument)
{
    xmlXPathContextPtr xpathEvaluationContext; 
    xmlXPathObjectPtr xpathObject; 
    xmlNodeSetPtr node;
    xmlNodePtr finalNode;
    char* stringCharacter = (char*)malloc(16 * sizeof(char));
    char* xpathExpresion = (char*)malloc(255 * sizeof(char));
    snprintf(stringCharacter, 16, "%d", pIndexFigure);
    strcpy(xpathExpresion, "//figura");
    strcat(xpathExpresion, stringCharacter);
    strcat(xpathExpresion, pFeature);
    xpathEvaluationContext = xmlXPathNewContext(pDocument);

    xpathObject = xmlXPathEvalExpression((xmlChar*)xpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    
    if(!strcmp(pFeature, "/TipoScheduler"))
    {
        if(pNewValue == 1)
        {
            memset(stringCharacter, 0, 16);
            strcpy(stringCharacter, "RoundRobin");
            xmlNodeSetContent(finalNode, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
        }
        else
        {
            memset(stringCharacter, 0, 16);
            strcpy(stringCharacter, "Sort");
            xmlNodeSetContent(finalNode, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
        }
    }
    else
    {
        memset(stringCharacter, 0, 16);
        snprintf(stringCharacter, 16, "%d", pNewValue);
        xmlNodeSetContent(finalNode, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    }

    xmlSaveFileEnc("figuras.xml", pDocument, "UTF-8");

    free(xpathExpresion);
    free(stringCharacter);
    xmlXPathFreeObject(xpathObject);
    xmlXPathFreeContext(xpathEvaluationContext); 
    xmlFreeDoc(pDocument);
}

void insertNewConnection(char *pIp, int pPort, xmlDocPtr pDocument)
{
    xmlNodePtr currentRoot, counter, root, ip, port;
    char *value;
    int newValue;
    char* stringCharacter = (char*)malloc(16 * sizeof(char));
    char* name = (char*)malloc(32 * sizeof(char));

    currentRoot = xmlDocGetRootElement(pDocument);
    counter = currentRoot->children;
    value = (char*)xmlNodeGetContent(counter);
    newValue = atoi(value) + 1;

    snprintf(stringCharacter, 16, "%d", newValue);

    strcpy(name, "conexion");
    strcat(name, stringCharacter);

    xmlNodeSetContent(counter, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));

    root = xmlNewNode(NULL, BAD_CAST name);
    ip = xmlNewChild(root, NULL, BAD_CAST "IP", NULL);
    port = xmlNewChild(root, NULL, BAD_CAST "Puerto", NULL);

    xmlNodeSetContent(ip, xmlEncodeSpecialChars(pDocument, BAD_CAST pIp));
    snprintf(stringCharacter, 16, "%d", pPort);
    xmlNodeSetContent(port, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    
    xmlAddChild(currentRoot, root);

    xmlSaveFileEnc("conexiones.xml", pDocument, "UTF-8");
    free(stringCharacter);
    free(name);
    free(pIp);
    xmlFreeDoc(pDocument);
    xmlCleanupParser();
}

void insertNewSettings(int pQuantum, int pFiguresDelay, int pExplosionVelocity, xmlDocPtr pDocument)
{
    xmlNodePtr currentRoot, quantum, figuresDelay, explosionVelocity;
    char* stringCharacter = (char*)malloc(16 * sizeof(char));

    currentRoot = xmlDocGetRootElement(pDocument);

    for(xmlNodePtr node = currentRoot->children; node != NULL; node = currentRoot->children)
    {
        xmlUnlinkNode(node);
        xmlFreeNode(node);
    }

    quantum = xmlNewChild(currentRoot, NULL, BAD_CAST "Quantum", NULL);
    figuresDelay = xmlNewChild(currentRoot, NULL, BAD_CAST "RetrasoFigura", NULL);
    explosionVelocity = xmlNewChild(currentRoot, NULL, BAD_CAST "VelocidadExplosion", NULL);

    snprintf(stringCharacter, 16, "%d", pQuantum);
    xmlNodeSetContent(quantum, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    snprintf(stringCharacter, 16, "%d", pFiguresDelay);
    xmlNodeSetContent(figuresDelay, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));
    snprintf(stringCharacter, 16, "%d", pExplosionVelocity);
    xmlNodeSetContent(explosionVelocity, xmlEncodeSpecialChars(pDocument, BAD_CAST stringCharacter));

    xmlSaveFileEnc("configuracion.xml", pDocument, "UTF-8");
    free(stringCharacter);
    xmlFreeDoc(pDocument);
    xmlCleanupParser();
}

int getCounter(xmlDocPtr pDocument)
{
	xmlXPathContextPtr xpathEvaluationContext; 
    xmlXPathObjectPtr xpathObject; 
    xmlNodeSetPtr node;
    xmlNodePtr finalNode;
    int result;
    char* xpathExpresion = (char*)malloc(255 * sizeof(char));
    strcpy(xpathExpresion, "//Contador");
    xpathEvaluationContext = xmlXPathNewContext(pDocument);
    xpathObject = xmlXPathEvalExpression((xmlChar*)xpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    result = atoi((char*)xmlNodeGetContent(finalNode));

    free(xpathExpresion);
    xmlXPathFreeObject(xpathObject);
    xmlXPathFreeContext(xpathEvaluationContext); 
    xmlFreeDoc(pDocument);

    return result;
}

ASCIIFigure extractFigure(xmlDocPtr pDocument, char pIndexFigure)
{
	xmlXPathContextPtr xpathEvaluationContext; 
    xmlXPathObjectPtr xpathObject; 
    xmlNodeSetPtr node;
    xmlNodePtr finalNode;
    char* xpathExpresion = (char*)malloc(255 * sizeof(char));
    char *newXpathExpresion = (char*)malloc(255 * sizeof(char));
    ASCIIFigure newFigure = (ASCIIFigure)malloc(sizeof(struct ASCIIArt));;
    strcpy(xpathExpresion, "//figura");
    strcat(xpathExpresion, &pIndexFigure);
    xpathEvaluationContext = xmlXPathNewContext(pDocument);

    xpathObject = xmlXPathEvalExpression((xmlChar*)xpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectName = (char*)malloc(sizeof(char) * 32);;
    strcpy(newFigure->objectName, (char*)finalNode->name);

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/TipoScheduler");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->figureType = (char*)malloc(sizeof(char) * 11);
    strcpy(newFigure->figureType, (char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/TiempoLimiteInicial");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->limitTime[0] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/TiempoLimiteFinal");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->limitTime[1] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AnchoCanvas");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->canvasSize[0] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AlturaCanvas");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->canvasSize[1] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AnchoFigura0");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectSize0[0] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AlturaFigura0");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectSize0[1] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AnchoFigura90");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectSize90[0] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AlturaFigura90");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectSize90[1] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AnchoFigura180");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectSize180[0] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AlturaFigura180");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectSize180[1] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AnchoFigura270");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectSize270[0] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AlturaFigura270");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->objectSize270[1] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/PosicionInicialX");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->position[0] = atoi((char*)xmlNodeGetContent(finalNode));
    newFigure->currentPosition[0] = newFigure->position[0];

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/PosicionInicialY");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->position[1] = atoi((char*)xmlNodeGetContent(finalNode));
    newFigure->currentPosition[1] = newFigure->position[1];

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/PosicionFinalX");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->position[2] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/PosicionFinalY");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->position[3] = atoi((char*)xmlNodeGetContent(finalNode));

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AnguloInicial");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->angle[0] = atoi((char*)xmlNodeGetContent(finalNode));
    newFigure->currentAngle = newFigure->angle[0];

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/AnguloFinal");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    newFigure->angle[1] = atoi((char*)xmlNodeGetContent(finalNode));

    free(xpathExpresion);
    free(newXpathExpresion);
    xmlXPathFreeObject(xpathObject);
    xmlXPathFreeContext(xpathEvaluationContext); 
    xmlFreeDoc(pDocument);
    return newFigure;
}

char *extractIP(xmlDocPtr pDocument, char *pIndexConnection)
{
    xmlXPathContextPtr xpathEvaluationContext; 
    xmlXPathObjectPtr xpathObject; 
    xmlNodeSetPtr node;
    xmlNodePtr finalNode;
    char* xpathExpresion = (char*)malloc(255 * sizeof(char));
    char *newXpathExpresion = (char*)malloc(255 * sizeof(char));
    char *ip = (char*)malloc(sizeof(char) * 20);
    strcpy(xpathExpresion, "//conexion");
    strcat(xpathExpresion, pIndexConnection);
    xpathEvaluationContext = xmlXPathNewContext(pDocument);

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/IP");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    strcpy(ip, (char*)xmlNodeGetContent(finalNode));

    free(xpathExpresion);
    free(newXpathExpresion);
    xmlXPathFreeObject(xpathObject);
    xmlXPathFreeContext(xpathEvaluationContext); 
    xmlFreeDoc(pDocument);
    return ip;
}

int extractPort(xmlDocPtr pDocument, char *pIndexConnection)
{
    xmlXPathContextPtr xpathEvaluationContext; 
    xmlXPathObjectPtr xpathObject; 
    xmlNodeSetPtr node;
    xmlNodePtr finalNode;
    char* xpathExpresion = (char*)malloc(255 * sizeof(char));
    char *newXpathExpresion = (char*)malloc(255 * sizeof(char));
    strcpy(xpathExpresion, "//conexion");
    strcat(xpathExpresion, pIndexConnection);
    xpathEvaluationContext = xmlXPathNewContext(pDocument);

    strcpy(newXpathExpresion, xpathExpresion);
    strcat(newXpathExpresion, "/Puerto");
    xpathObject = xmlXPathEvalExpression((xmlChar*)newXpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    int port = atoi((char*)xmlNodeGetContent(finalNode));;

    free(xpathExpresion);
    free(newXpathExpresion);
    xmlXPathFreeObject(xpathObject);
    xmlXPathFreeContext(xpathEvaluationContext); 
    xmlFreeDoc(pDocument);
    return port;
}

int extractSettings(xmlDocPtr pDocument, int pIndexSetting)
{
    xmlXPathContextPtr xpathEvaluationContext; 
    xmlXPathObjectPtr xpathObject; 
    xmlNodeSetPtr node;
    xmlNodePtr finalNode;

    char* xpathExpresion = (char*)malloc(255 * sizeof(char));

    if(pIndexSetting == 0)
    {
        strcpy(xpathExpresion, "//Quantum");
    }
    else if(pIndexSetting == 1)
    {
        strcpy(xpathExpresion, "//RetrasoFigura");
    }
    else if(pIndexSetting == 2)
    {
        strcpy(xpathExpresion, "//VelocidadExplosion");
    }

    xpathEvaluationContext = xmlXPathNewContext(pDocument);

    xpathObject = xmlXPathEvalExpression((xmlChar*)xpathExpresion, xpathEvaluationContext);
    node = xpathObject->nodesetval;
    finalNode = node->nodeTab[0];
    int value = atoi((char*)xmlNodeGetContent(finalNode));;

    free(xpathExpresion);
    xmlXPathFreeObject(xpathObject);
    xmlXPathFreeContext(xpathEvaluationContext); 
    xmlFreeDoc(pDocument);
    return value;
}
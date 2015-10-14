/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#include "PrincipalMenu.h"
#include "Animation.h"
#include "XMLParser.h"
#include "ReceiverMode.h"

static void createFigure();
static void modifyFigure();
static void printFigure();
static void selectFigure();
static void initialAnimationStructure();
static void createConnection();
static void printConnections();
static void createSettingsParameters();
static void printSettings();

int structuredCreated = 0;

void principalMenu()
{
	int seleccion = 0;
	int controlPrincipal = 1;
	while(controlPrincipal)
	{
		erase();
		printw("1) Conexión.\n");
		printw("2) Figuras.\n");
		printw("3) Animación.\n");
		printw("4) Modo Receptor.\n");
		printw("5) Configuracion.\n");
		printw("6) Salir.\n\n");
		printw("Por favor selecciona una opción disponible: ");
		scanw("%d", &seleccion);
		refresh();
		if(seleccion == 1)
		{
			int controlOne = 1;
			while(controlOne)
			{
				seleccion = 0;
				erase();
				printw("1) Crear nueva conexion.\n");
				printw("2) Borrar conexion.\n");
				printw("3) Ver conexiones actuales.\n");
				printw("4) Volver.\n\n");
				printw("Por favor selecciona una opción disponible: ");
				scanw("%d", &seleccion);
				refresh();
				if(seleccion == 1)
				{
					createConnection();
				}
				else if(seleccion == 2)
				{
					erase();
					printw("Caracteristica no implementada...");
					refresh();
					getch();
				}
				else if(seleccion == 3)
				{
					printConnections();
				}
				else if(seleccion == 4)
				{
					controlOne = 0;
				}
				else
				{
					printw("Por favor selecciona una opción valida.\n");
					refresh();
					getch();
				}
			}
		}
		else if(seleccion == 2)
		{
			int controlTwo = 1;
			while(controlTwo)
			{
				seleccion = 0;
				erase();
				printw("1) Crear nueva figura.\n");
				printw("2) Modificar figura.\n");
				printw("3) Borrar figura.\n");
				printw("4) Ver datos de figuras actuales.\n");
				printw("5) Volver.\n\n");
				printw("Por favor selecciona una opción disponible: ");
				scanw("%d", &seleccion);
				refresh();
				if(seleccion == 1)
				{
					createFigure();
				}
				else if(seleccion == 2)
				{
					modifyFigure();
				}
				else if(seleccion == 3)
				{
					erase();
					printw("Caracteristica no implementada...");
					refresh();
					getch();
				}
				else if(seleccion == 4)
				{
					printFigure();
				}
				else if(seleccion == 5)
				{
					controlTwo = 0;
				}
				else
				{
					printw("Por favor selecciona una opción valida.\n");
					refresh();
					getch();
				}
			}
		}
		else if(seleccion == 3)
		{
			int controlThree = 1;
			while(controlThree)
			{
				seleccion = 0;
				erase();
				printw("1) Seleccionar figura para animación.\n");
				printw("2) Iniciar animación.\n");
				printw("3) Volver.\n\n");
				printw("Por favor selecciona una opción disponible: ");
				scanw("%d", &seleccion);
				refresh();
				if(seleccion == 1)
				{
					selectFigure();
				}
				else if(seleccion == 2)
				{
					startAnimation();
				}
				else if(seleccion == 3)
				{
					controlThree = 0;
				}
				else
				{
					printw("Por favor selecciona una opción valida.\n");
					refresh();
					getch();
				}
			}
		}
		else if(seleccion == 4)
		{
			int puerto = 0;
			erase();
			printw("Puerto donde desea colocar el receptor: ");
			scanw("%d", &puerto);
			refresh();
			startReceiverMode(puerto);
		}
		else if(seleccion == 5)
		{
			int controlSettings = 1;
			while(controlSettings)
			{
				seleccion = 0;
				erase();
				printw("1) Crear nuevos parametros de configuracion.\n");
				printw("2) Ver parametros de configuracion actuales.\n");
				printw("3) Volver.\n\n");
				printw("Por favor selecciona una opción disponible: ");
				scanw("%d", &seleccion);
				refresh();
				if(seleccion == 1)
				{
					createSettingsParameters();
				}
				else if(seleccion == 2)
				{
					printSettings();
				}
				else if(seleccion == 3)
				{
					controlSettings = 0;
				}
				else
				{
					printw("Por favor selecciona una opción valida.\n");
					refresh();
					getch();
				}
			}
		}
		else if(seleccion == 6)
		{
			controlPrincipal = 0;
		}
		else
		{
			printw("Por favor selecciona una opción valida.\n");
			refresh();
			getch();
		}
	}
}

static void createSettingsParameters()
{
	int quantum, figuresDelay, explosionVelocity;
	erase();
	printw("Quantum: ");
	scanw("%d", &quantum);
	refresh();
	printw("Retraso de figuras: ");
	scanw("%d", &figuresDelay);
	refresh();
	printw("Velocidad de explosion: ");
	scanw("%d", &explosionVelocity);
	refresh();
	insertNewSettings(quantum, figuresDelay, explosionVelocity, createDocumentXML("configuracion.xml"));
}

static void printSettings()
{
	erase();
	printw(" ");
	system("sudo iceweasel configuracion.xml");
	refresh();
}

static void createConnection()
{
	erase();
	int port;
	char *ip = (char*)malloc(sizeof(char) * 20);
	printw("IP: ");
	scanw("%s", ip);
	refresh();
	printw("Puerto: ");
	scanw("%d", &port);
	refresh();
	insertNewConnection(ip, port, createDocumentXML("conexiones.xml"));
}

static void createFigure()
{
	erase();
	int initialLimitTime, finalLimitTime, widthCanvasSize, heightCanvasSize, XInitialPosition, YInitialPosition, XFinalPosition, YFinalPosition, initialAngle, finalAngle;
	int widthFigure0, heightFigure0, widthFigure90, heightFigure90, widthFigure180, heightFigure180, widthFigure270, heightFigure270;
	char *objectName, *figureType;
	printw("Tiempo limite inicial: ");
	scanw("%d", &initialLimitTime);
	refresh();
	printw("Tiempo limite final: ");
	scanw("%d", &finalLimitTime);
	refresh();
	printw("Ancho de canvas: ");
	scanw("%d", &widthCanvasSize);
	refresh();
	printw("Altura de canvas: ");
	scanw("%d", &heightCanvasSize);
	refresh();
	objectName = (char*)malloc(sizeof(char) * 32);
	printw("Nombre del archivo que contiene la figura: ");
	scanw("%s", objectName);
	refresh();
	figureType = (char*)malloc(sizeof(char) * 11);
	printw("Tipo de scheduler que administrará la figura (RoundRobin, Sort): ");
	scanw("%s", figureType);
	refresh();
	printw("Ancho de figura a 0 grados: ");
	scanw("%d", &widthFigure0);
	refresh();
	printw("Alto de figura a 0 grados: ");
	scanw("%d", &heightFigure0);
	refresh();
	printw("Ancho de figura a 90 grados: ");
	scanw("%d", &widthFigure90);
	refresh();
	printw("Alto de figura a 90 grados: ");
	scanw("%d", &heightFigure90);
	refresh();
	printw("Ancho de figura a 180 grados: ");
	scanw("%d", &widthFigure180);
	refresh();
	printw("Alto de figura a 180 grados: ");
	scanw("%d", &heightFigure180);
	refresh();
	printw("Ancho de figura a 270 grados: ");
	scanw("%d", &widthFigure270);
	refresh();
	printw("Alto de figura a 270 grados: ");
	scanw("%d", &heightFigure270);
	refresh();
	printw("Posición X inicial: ");
	scanw("%d", &XInitialPosition);
	refresh();
	printw("Posición Y inicial: ");
	scanw("%d", &YInitialPosition);
	refresh();
	printw("Posicion X final: ");
	scanw("%d", &XFinalPosition);
	refresh();
	printw("Posición Y final: ");
	scanw("%d", &YFinalPosition);
	refresh();
	printw("Angulo inicial: ");
	scanw("%d", &initialAngle);
	refresh();
	printw("Angulo final: ");
	scanw("%d", &finalAngle);
	refresh();
	ASCIIFigure newAnimation = (ASCIIFigure)malloc(sizeof(struct ASCIIArt));
	newAnimation->limitTime[0] = initialLimitTime;
	newAnimation->limitTime[1] = finalLimitTime;
	newAnimation->canvasSize[0] = widthCanvasSize;
	newAnimation->canvasSize[1] = heightCanvasSize;
	newAnimation->objectName = objectName;
	newAnimation->figureType = figureType;
	newAnimation->objectSize0[0] = widthFigure0;
	newAnimation->objectSize0[1] = heightFigure0;
	newAnimation->objectSize90[0] = widthFigure90;
	newAnimation->objectSize90[1] = heightFigure90;
	newAnimation->objectSize180[0] = widthFigure180;
	newAnimation->objectSize180[1] = heightFigure180;
	newAnimation->objectSize270[0] = widthFigure270;
	newAnimation->objectSize270[1] = heightFigure270;
	newAnimation->position[0] = XInitialPosition;
	newAnimation->position[1] = YInitialPosition;
	newAnimation->position[2] = XFinalPosition;
	newAnimation->position[3] = YFinalPosition;
	newAnimation->angle[0] = initialAngle;
	newAnimation->angle[1] = finalAngle;
	insertNewFigure(newAnimation, createDocumentXML("figuras.xml"));
}

static void modifyFigure()
{
	int controlModify = 1;
	int figure = 0; 
	erase();
	printw("Digite el indice de la figura que desea modificar: ");
	scanw("%d", &figure);
	refresh();
	while(controlModify)
	{
		int seleccion = 0;
		int newValue = 0;
		erase();
		printw("1) Tiempo limite inicial.\n");
		printw("2) Tiempo limite final.\n");
		printw("3) Ancho de canvas.\n");
		printw("4) Altura de canvas.\n");
		printw("5) Scheduler.\n");
		printw("6) Ancho de figura a 0 grados.\n");
		printw("7) Altura de figura a 0 grados.\n");
		printw("8) Ancho de figura a 90 grados.\n");
		printw("9) Altura de figura a 90 grados.\n");
		printw("10) Ancho de figura a 180 grados.\n");
		printw("11) Altura de figura a 180 grados.\n");
		printw("12) Ancho de figura a 270 grados.\n");
		printw("13) Altura de figura a 270 grados.\n");
		printw("14) Posicion X inicial.\n");
		printw("15) Posicion Y inicial.\n");
		printw("16) Posicion X final.\n");
		printw("17) Posicion Y final.\n");
		printw("18) Angulo inicial.\n");
		printw("19) Angulo final.\n");
		printw("20) Volver.\n\n");
		printw("Por favor selecciona una opción disponible: ");
		scanw("%d", &seleccion);
		refresh();
		switch(seleccion)
		{
			case 1:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/TiempoLimiteInicial", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 2:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/TiempoLimiteFinal", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 3:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AnchoCanvas", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 4:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AlturaCanvas", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 5:
				erase();
				printw("Por favor digite el nuevo valor (1 = RoundRobin, 2 = Sort): ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/TipoScheduler", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 6:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AnchoFigura0", newValue, createDocumentXML("figuras.xml"));	
			    break;
			case 7:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AlturaFigura0", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 8:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AnchoFigura90", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 9:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AlturaFigura90", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 10:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AnchoFigura180", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 11:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AlturaFigura180", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 12:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AnchoFigura270", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 13:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AlturaFigura270", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 14:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/PosicionInicialX", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 15:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/PosicionInicialY", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 16:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/PosicionFinalX", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 17:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/PosicionFinalY", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 18:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AnguloInicial", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 19:
				erase();
				printw("Por favor digite el nuevo valor: ");
				scanw("%d", &newValue);
				refresh();
				modifyFigureXML(figure, "/AnguloFinal", newValue, createDocumentXML("figuras.xml"));
			    break;
			case 20:
				controlModify = 0;
				break;
			default:
				erase();
				printf("Por favor digite una opcion valida...");
				refresh();
				getch();
			    break;
		}
	}
}

static void printFigure()
{
	erase();
	printw(" ");
	system("sudo iceweasel figuras.xml");
	refresh();
}

static void printConnections()
{
	erase();
	printw(" ");
	system("sudo iceweasel conexiones.xml");
	refresh();
}

static void initialAnimationStructure()
{
    if(!structuredCreated)
    {
    	createArrayAnimation(getCounter(createDocumentXML("figuras.xml")));
        structuredCreated = 1;
    }
}

static void selectFigure()
{
	erase();
	char selectFigure;
	printw("Digite el numero de figura que desea agregar (ejemp: 1, 2...): ");
	scanw("%c", &selectFigure);
	refresh();
	initialAnimationStructure();
	insertFigureToAnimation(extractFigure(createDocumentXML("figuras.xml"), selectFigure));
}
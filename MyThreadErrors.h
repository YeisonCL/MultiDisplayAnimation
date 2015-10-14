/*********************************************/
/*		Instituto Tecnológico de Costa Rica  */
/*		Ingeniería en Computadores           */
/*		Principios de Sistemas Operativos    */
/*		II Semestre 2015                     */
/*		                                     */
/*		Author: Yeison Arturo Cruz León      */
/*		Carné: 201258348                     */
/*********************************************/

#ifndef _MYTHREADERRORS_H
#define	_MYTHREADERRORS_H

enum ThreadErros
{
	//Valor retornado si todo salio bien.
    SUCESS = 0,
    //Este valor es retornado si la funcion solicitada no puede ser realizada con los parametros dados.
    INVALID_OPERATION = -1,
    //Valor retornado si cualquier funcion de la biblioteca "MyThread" es llamada sin haber llamado primero a "myThreadInit"
    MY_THREAD_NOT_INITIALIZED = -2,
    //Este valor es retornado si en algun momento no hay memoria suficiente para realizar la funcion solicitada.
    NOT_ENOUGH_MEMORY = -3,
    //Este valor es retornado si el parametro dado de "thread_t" es invalido o no ha sido previamente inicializado
    INVALID_THREAD_T = -4
};

#endif
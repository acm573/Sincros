/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Archivo:					PRE_EventosPantallaPrincipal.c
. Prop�sito:				Administra los eventos que se generen en la
.							pantalla principal de la aplicaci�n
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		18 de Febrero de 2013
. Responsable modificaci�n: 
.
*******************************************************************************/
#include <userint.h>
#include "PRE_Principal.h"
#include "pre_variables.h"

//prototipo de funciones
int (*miFuncionPRE)(void);


/*****************************************************************************
.
. Funci�n C:			PRE_SeleccionPantallaPrincipal
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura los eventos registrados sobre los controles
.						del panel iPanelPrincipal
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_SeleccionPantallaPrincipal (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE:
			PRE_ResaltarOpcion(panel, control,pMenu_cnvFondo,1, pPrincipal_txtEstado);
			break;
			
		case EVENT_LEFT_CLICK_UP:
			
			break;
	}
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			PRI_LLamadasRemotas
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Permite configurar mediante un timer, hacer una
.						llamada a una funci�n pasada por par�metro que no
.						requiera de argumentos.
. Par�metro de entrada:	int (*funcionPaso)(void)) | nombre de la funci�n que
.										se desea sea llamada por el timer al
.										cumplir el primer TICK.
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRI_LlamadasRemotas(int (*funcionPaso)(void))
{
	miFuncionPRE = funcionPaso;
	SetCtrlAttribute(iPanelPrincipal, pPrincipal_tmrLlamadas, ATTR_ENABLED, 1);
	
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRI_Llamadas
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Evento callback del timer que sirve para retardar la
.						llamada a la funci�n que fue configurada por la
.						funci�n PRI_LlamadasRemotas.
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRI_Llamadas (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			SetCtrlAttribute(panel, control, ATTR_ENABLED, 0);
			miFuncionPRE();
			break;
	}
	return 0;
}

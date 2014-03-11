/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Archivo:					PRE_Principal.c
. Propósito:				Es el punto de entrada a la aplicación. Desde aqui
.							se gestiona el lanzamiento de la pantalla principal
.							de la aplicacion de control del sistema manipulador.
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		18 de Febrero de 2013
. Responsable modificación: 
.
*******************************************************************************/
#include <cvirte.h>		
#include <userint.h>
#include "pre_variables.h"


/*****************************************************************************
.
. Función C:			main
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Función de entrada para iniciar el sistema de control.
.						Establece la secuencia de las llamadas a las funciones
.						que configuran y establecen las condiciones de inicio
.						del sistema de control de manipulador.
. Parámetro de entrada:	int argc | numero de parametros
.						char *argv[] | arreglo con los parametros
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int main (int argc, char *argv[])
{
	//inicializalas referencias de todos los paneles
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((iPanelPrincipal = LoadPanel (0, "PRE_Principal.uir", pPrincipal)) < 0)
		return -1;
	
	if ((iPanelMenuPrincipal = LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pMenu)) < 0)
		return -1;
	
	if ((iPanelCatTransmisiones = LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pCatTransm)) < 0)
		return -1;
	
	if ((iPanelEntrenamiento= LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pEntrenar)) < 0)
		return -1;

	//inicializa la interfaz de usuario
	PRE_InicializarInterfaz();
	
	//establece las condiciones de inicio
	PRE_InicializarSistema();
	
	DisplayPanel (iPanelPrincipal);
	
	RunUserInterface ();
	DiscardPanel (iPanelPrincipal);
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_Principal
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Función que captura los eventos en el panel principal
.						de la aplicación del sistema
. Parámetro de entrada:	los de una función callback
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_Principal (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			PRE_FinalizarAplicacion();
			break;
		case EVENT_PANEL_SIZING:
			PRE_AjustarComponentes(eventData2);
			break;
		case EVENT_PANEL_SIZE:
			PRE_AjustarComponentes(eventData2);
			break;
		case EVENT_LEFT_CLICK:
			PRE_AnimacionMenu(OCULTAR);
			break;
	}
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_MostrarMenu
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Función que captura los eventos sobre el botón del
.						menu principal de la aplicación.
. Parámetro de entrada:	los de una función callback
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_MostrarMenu (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			PRE_ConfiguraMenu();
			break;
			
		case EVENT_MOUSE_POINTER_MOVE:
			PRE_BarraEstado("Despliega el menú principal del sistema.");
			break;
	}
	return 0;
}



/*****************************************************************************
.
. Función C:			PRE_FinalizarAplicacion
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Función que captura la solicitud de cerrar la 
.						aplicación del sistema de control.
. Parámetro de entrada:	los de una función callback
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_FinalizarAplicacion()
{
	if (ConfirmPopup ("Finalizar sesión con el sistema",
				  "¿Desea cerrar la aplicación?")==1)
	{
		PRE_Finalizar();
		QuitUserInterface (0);	
	}   
	return 0;
}





/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Archivo:					PRE_Principal.c
. Prop�sito:				Es el punto de entrada a la aplicaci�n. Desde aqui
.							se gestiona el lanzamiento de la pantalla principal
.							de la aplicacion de control del sistema manipulador.
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		18 de Febrero de 2013
. Responsable modificaci�n: 
.
*******************************************************************************/
#include <cvirte.h>		
#include <userint.h>
#include "pre_variables.h"


/*****************************************************************************
.
. Funci�n C:			main
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Funci�n de entrada para iniciar el sistema de control.
.						Establece la secuencia de las llamadas a las funciones
.						que configuran y establecen las condiciones de inicio
.						del sistema de control de manipulador.
. Par�metro de entrada:	int argc | numero de parametros
.						char *argv[] | arreglo con los parametros
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
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
. Funci�n C:			PRE_Principal
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Funci�n que captura los eventos en el panel principal
.						de la aplicaci�n del sistema
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
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
. Funci�n C:			PRE_MostrarMenu
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Funci�n que captura los eventos sobre el bot�n del
.						menu principal de la aplicaci�n.
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
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
			PRE_BarraEstado("Despliega el men� principal del sistema.");
			break;
	}
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_FinalizarAplicacion
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Funci�n que captura la solicitud de cerrar la 
.						aplicaci�n del sistema de control.
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_FinalizarAplicacion()
{
	if (ConfirmPopup ("Finalizar sesi�n con el sistema",
				  "�Desea cerrar la aplicaci�n?")==1)
	{
		PRE_Finalizar();
		QuitUserInterface (0);	
	}   
	return 0;
}





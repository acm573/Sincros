/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Archivo:					PRE_EventosMenuPrincipal.c
. Prop�sito:				Administra los mensajes de ayuda que se despliegan
.							en la pantalla 
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		18 de Febrero de 2013
. Responsable modificaci�n: 
.
*******************************************************************************/
#include "toolbox.h"
#include "pre_variables.h"


/*****************************************************************************
.
. Funci�n C:			PRE_SeleccionMenu
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura los eventos registrados sobre los controles
.						del panel iPanelMenuPrincipal
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_SeleccionMenu (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE:
			PRE_ResaltarOpcion(panel, control,pMenu_cnvFondo,2, pMenu_cnvFondoPanel, pMenu_picCerrar);
			break;
		
			
		case EVENT_LEFT_CLICK_UP:
			
			if (control == pMenu_picCerrar)
			{
				PRE_AnimacionMenu(OCULTAR);
			}
			
			if (control == pMenu_picSalir)
			{
				PRE_AnimacionMenu(OCULTAR);
				PRE_FinalizarAplicacion();
			}
			
			if (control == pMenu_picTransmisiones)
			{
				SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_cnvFondo, ATTR_LEFT, -1000);
				PRE_UbicarPanel(iPanelCatTransmisiones);
			}
			
			if (control == pMenu_picEntrenamiento)
			{
				SetCtrlAttribute(iPanelEntrenamiento, pEntrenar_cnvFondo, ATTR_LEFT, -1000);
				PRE_IniciarEntrenamiento();
			}
			break;
	}
	return 0;
}







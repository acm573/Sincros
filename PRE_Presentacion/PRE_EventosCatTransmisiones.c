/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Archivo:					PRE_EventosCatTransmisiones.c
. Prop�sito:				Contiene las funciones referentes al proceso de 
.							eventos de transmisiones.
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		18 de Febrero de 2013
. Responsable modificaci�n: 
.
*******************************************************************************/
#include "toolbox.h"
#include <userint.h>
#include "PRE_Principal.h"
#include "pre_variables.h"


int BDS_LeerDetalleTransmision(int iPanel, ...);
int BDS_LeerTransmisiones(int iPanel, int iControl);
int PRE_CambioTransmision(int iPanel, int iControl);

/*****************************************************************************
.
. Funci�n C:			PRE_PanelCatTransmisiones
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura los eventos registrados sobre el panel 
.						iPanelCatTransmisiones
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_PanelCatTransmisiones (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			PRE_AnimacionMenu(OCULTAR);
			break;
	}
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			PRE_SeleccionCatTransmisiones
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura los eventos registrados sobre los controles 
.						del panel iPanelCatTransmisiones
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_SeleccionCatTransmisiones (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	if (event == EVENT_MOUSE_POINTER_MOVE)
	{
		PRE_ResaltarOpcion(panel, control, pCatTransm_cnvFondo, 3, pCatTransm_lstTransmisiones, 
			pCatTransm_numNumeroVelocidades, pCatTransm_txtNombreTransmision);
	}
	
	if (event == EVENT_LEFT_CLICK_UP)
	{
		if (control == pCatTransm_picCerrar)
		{
			PRE_OcultarPanel(panel);
		}
	}
	
	if (event == EVENT_LEFT_CLICK_UP)
	{
		PRE_CambioTransmision(panel, control);
	}
	
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_IniciarCatTransmisiones
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Una vez que el usuario ha solicitado mostrar el 
.						catalogo de transmisiones, se despliega en pantalla
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	06 de Marzo de 2014
.
*****************************************************************************/
int PRE_IniciarCatTransmisiones()
{
	stEntrenar Modo = CTRL_SIN_TRANSMISIONES;
	
	PRE_UbicarPanel(iPanelCatTransmisiones);
	
	if (BDS_LeerTransmisiones(iPanelCatTransmisiones, pCatTransm_lstTransmisiones) > 0)
	{
		PRE_CambioTransmision(iPanelCatTransmisiones, pCatTransm_lstTransmisiones);
	}
	
	//PRE_ControlesEntrenamiento(Modo);
	//PRE_CondicionesIniciales();
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_CambioTransmision
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Procesa el cambio de selecci�n de una transmisi�n en
.						la lista que muestra las que se han configurado.
.						Su efecto es actualizar la informaci�n en la pantalla
.						con la infomaci+�n en detalle de la mista.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	06 de Marzo de 2014
.
*****************************************************************************/
int PRE_CambioTransmision(int iPanel, int iControl)
{
	int iId=0;	
	
	GetCtrlVal(iPanelCatTransmisiones, pCatTransm_lstTransmisiones, &iId);
	BDS_LeerDetalleTransmision(iPanelCatTransmisiones, pCatTransm_txtNombreTransmision,
				pCatTransm_numNumeroVelocidades, pCatTransm_tblRelaciones, iId);
		
	return 0;
}

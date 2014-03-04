/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Industrial
. Archivo:					PRE_Generales.c
. Prop�sito:				Funciones generales y de uso com�n en el sistema
.							de control.
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		18 de Febrero de 2013
. Responsable modificaci�n: 
.
*******************************************************************************/
#include "toolbox.h"
#include <ansi_c.h>
#include "pre_variables.h"
#include "stdarg.h"


/*****************************************************************************
.
. Funci�n C:			PRE_ResaltarOpcion
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Ayuda en el proceso de indicar que una opci�n de la 
.						pantalla, ha sido seleccionada con el cursor.
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_ResaltarOpcion(int iPanel, int iControl, int iFondo, int iCantidad, ...)
{
	int iTop = 0;
	int iLeft = 0;
	int *iLista;
	va_list pa;
	int iExiste = 0;
	
	va_start(pa, iCantidad);
	
	//Crea la lista en la cual colocar� cada una de las referencias de los controles
	//y verifica que el par�metro iControl es diferente a lo que hay en iLista
	iLista = malloc(sizeof(int)*iCantidad);
	
	for (int i=0;i<iCantidad;i++)
	{
		iLista[i] = va_arg(pa, int);
		if (iControl == iLista[i])
		{
			iExiste=1;
			break;
		}
	}
	
	if (iExiste == 0)
	{
		GetCtrlAttribute(iPanel, iControl, ATTR_TOP, &iTop);
		GetCtrlAttribute(iPanel, iControl, ATTR_LEFT, &iLeft);
	
		SetCtrlAttribute(iPanel, iFondo, ATTR_TOP, iTop);
		SetCtrlAttribute(iPanel, iFondo, ATTR_LEFT, iLeft);
	}
	
	free(iLista);
	
	//Llama al proceso que pinta el mensaje de ayuda en pantalla
	PRI_MensajeControl(iPanel, iControl);

	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_Desmarcar
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Retira el control que muestra a una opci�n como 
.						seleccionada.
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_Desmarcar(int iControl)
{
	SetCtrlAttribute(PRE_PanelVisible(0), iControl, ATTR_LEFT, -1000);
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_OcultarPanel
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Implementa la animaci�n del panel de desoliegue de y
.						hacia la izquierda. 
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_OcultarPanel(int iPanel)
{
	int iLeft=0;
	int i=0;
	
	GetPanelAttribute(iPanel, ATTR_LEFT, &iLeft);
	
	i=0;
	while (i<50)
	{
		SetPanelAttribute(iPanel, ATTR_LEFT, iLeft-i);
		DelayWithEventProcessing(0.0);
		i+=10;
	}
	
	SetPanelAttribute (iPanel, ATTR_VSCROLL_OFFSET, 0);
	SetPanelAttribute (iPanel, ATTR_HSCROLL_OFFSET, 0);
	
	HidePanel(iPanel);
	
	PRE_PanelVisible(iPanel);	
	
	return 0;
}

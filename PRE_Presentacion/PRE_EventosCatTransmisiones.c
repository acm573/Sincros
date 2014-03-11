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
int PRE_OpcionNuevaTransimision(void);
int PRE_ActualizaTablaRelaciones(void);
int PRE_CancelarProceso(void);


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
		switch (control)
		{
			case pCatTransm_picCerrar:
				PRE_OcultarPanel(panel);
				break;
				
			case pCatTransm_picCancelar:
				PRE_CancelarProceso();
				break;
				
			case pCatTransm_picAgregar:
				PRE_OpcionNuevaTransimision();
				break;
				
			case pCatTransm_lstTransmisiones:
				PRE_CambioTransmision(panel, control);
				break;
		}
	}
	
	if (event == EVENT_VAL_CHANGED)
	{
		switch (control)
		{
			case pCatTransm_numNumeroVelocidades:
				PRE_ActualizaTablaRelaciones();
				break;
		}
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
	PRE_UbicarPanel(iPanelCatTransmisiones);
	
	if (BDS_LeerTransmisiones(iPanelCatTransmisiones, pCatTransm_lstTransmisiones) == TRA_CON_TRANSMISIONES)
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


/*****************************************************************************
.
. Funci�n C:			PRE_OpcionNuevaTransmision
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			El usuario ha solicitado capturar unan nueva 
.						transmision
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	11 de Marzo de 2014
.
*****************************************************************************/
int PRE_OpcionNuevaTransimision()
{
	//limpia el contenido de los controles
	SetCtrlVal(iPanelCatTransmisiones, pCatTransm_txtNombreTransmision, "");
	SetCtrlVal(iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, 1);
	DeleteTableRows (iPanelCatTransmisiones, pCatTransm_tblRelaciones, 1,
					 -1);
	
	//modifica el comportamiento de los controles
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picAgregar, ATTR_DIMMED, 1);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEditar, ATTR_DIMMED, 1);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEliminar, ATTR_DIMMED, 1);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCerrar, ATTR_DIMMED, 1);
	SetCtrlAttribute(iPanelPrincipal, pPrincipal_btnMenu, ATTR_DIMMED, 1);
	
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_txtNombreTransmision, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_tblRelaciones, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, ATTR_CTRL_MODE,
					  VAL_HOT);
	
	PRE_ActualizaTablaRelaciones();
	
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_CancelarProceso
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			El usuario ha solicitado cancelar el proceso
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	11 de Marzo de 2014
.
*****************************************************************************/
int PRE_CancelarProceso()
{
	//restablece los controles al comportamiento incial
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picAgregar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEditar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEliminar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCerrar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCancelar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelPrincipal, pPrincipal_btnMenu, ATTR_DIMMED, 0);	
	
	PRE_IniciarCatTransmisiones();	
	
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			PRE_ActualizaTablaRelaciones
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Cuando el usuario define un numero de velocodades,
.						se crean el n�mero de renglones correspondientes en la
.						tabla de relaciones. Se tiene cuidado cuando el usuario
.						modifica a un valor menor y previamente existen ya
.						renglones creados.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	11 de Marzo de 2014
.
*****************************************************************************/
int PRE_ActualizaTablaRelaciones()
{
	int iRenglonesActuales = 0;	//numero de velocidades actuales
	int iRenglonesUsuario = 0;	//numero de velocidades indicadas por usuario
	
	//obtiene primero el numero de renglones que pueda tener la tabla
	GetNumTableRows (iPanelCatTransmisiones, pCatTransm_tblRelaciones, &iRenglonesActuales);

	//obtiene el numero de velocidades indicadas por el usuario
	GetCtrlVal(iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, &iRenglonesUsuario);
	
	//vefifica si se necesita aplicar alguna validaci�n antes de crear los 
	//renglones en la tabla de relaciones
	if (iRenglonesActuales == 0)
	{
		//la tabla se encuentra vacia por lo que se procede a crear los renglones
		//sin validacion alguna
		InsertTableRows (iPanelCatTransmisiones, pCatTransm_tblRelaciones, 1,
						 iRenglonesUsuario, VAL_USE_MASTER_CELL_TYPE);
	}
	else
	{
		//ya existen renglones en la tabla, por lo que se debe analizar si se
		//deben insertar o eliminar renglones en la tabla de relaciones
		
		if (iRenglonesActuales < iRenglonesUsuario)
		{
			//se ha determinado que hacen falta mas renglones en la tabla de relaciones
			//por lo que se procede a insertar nuevos renglones al final
			//de la tabla de relaciones
			InsertTableRows (iPanelCatTransmisiones, pCatTransm_tblRelaciones, iRenglonesActuales,
						 iRenglonesUsuario-iRenglonesActuales, VAL_USE_MASTER_CELL_TYPE);
		}
		else
		{
			if (iRenglonesActuales != iRenglonesUsuario)
			{
				//se ha determinado que se deben eliminar renglones de la tabla
				//de relaciones
				DeleteTableRows (iPanelCatTransmisiones, pCatTransm_tblRelaciones, iRenglonesUsuario,
								 iRenglonesActuales-iRenglonesUsuario);
			}
		}
	}
	
	
	
	
	return 0;
}

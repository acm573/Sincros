/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Archivo:					PRE_MensajeControles.c
. Prop�sito:				Administra los mensajes de ayuda que se despliegan
.							en la pantalla 
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		18 de Febrero de 2013
. Responsable modificaci�n: 
.
*******************************************************************************/
#include <ansi_c.h>
#include "pre_variables.h"



/*****************************************************************************
.
. Funci�n C:			PRI_MensajeControl
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Carga la cadena de texto de ayuda, de acuerdo al 
.						panel y control sobre el que se encuentra el cursor.
. Par�metro de entrada:	int iPanel | referencia al panel
.						int iControl | referencia al control
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRI_MensajeControl(int iPanel, int iControl)
{
	char *pcMensaje="";
	
	/*
	 * Mensajes para los elementos del panel iPanelMenuPrincipal
	 */
	if (iPanel == iPanelMenuPrincipal)
	{
		switch (iControl)
		{
			case pMenu_picCerrar:
				pcMensaje = "Oculta el men� de opciones del sistema.";
				break;
				
			case pMenu_picTransmisiones:
				pcMensaje = "Permite dar mantenimiento al cat�logo de tranmisiones.";
				break;
				
			case pMenu_picEntrenamiento:
				pcMensaje = "Habilita la funci�n de entrenamiento de posiciones para una transmisi�n.";
				break;
				
			case pMenu_picSecuencias:
				pcMensaje = "Permite dise�ar las caracter�sticas de una secuencia de prueba.";
				break;
				
			case pMenu_picModoAutomatico:
				pcMensaje = "Da inicio a una secuencia de prueba, aplicando las caracter�sticas definidas por el operadr.";
				break;
				
			case pMenu_picModoManual:
				pcMensaje = "Muestra la interfaz de operaci�n con el banco en modo manual.";
				break;
				
			case pMenu_picCalibracion:
				pcMensaje = "Inicia el m�dulo de calibraci�n para los componentes de medici�n del sistema.";
				break;
				
			case pMenu_picRegistros:
				pcMensaje = "Permite recuperar la informaci�n almacenada de pruebas pasadas.";
				break;
				
			case pMenu_picSalir:
				pcMensaje = "Finaliza la sesi�n con el sistema.";
				break;
		}
	}
	
	
	/*
	 * Mensajes para los elementos del panel iPanelPrincipal
	 */
	if (iPanel == iPanelPrincipal)
	{
		switch (iControl)
		{
			case pPrincipal_txtEstado:
				pcMensaje = "Despliega informaci�n referente a las opciones disponibles en el sistema.";
				break;
		}
	}
	
	
	/*
	 * Mensajes para los elementos del panel iPanelCatTransmisiones
	 */
	if (iPanel == iPanelCatTransmisiones)
	{
		switch (iControl)
		{
			case pCatTransm_picAgregar:
				pcMensaje = "Permite capturar la informaci�n que describe a una nueva transmisi�n.";
				break;
				
			case pCatTransm_picEditar:
				pcMensaje = "Habilita la informaci�n de una transmisi�n para que sea actualizada.";
				break;
				
			case pCatTransm_picEliminar:
				pcMensaje = "Elimina del sistema a la transmisi�n seleccionada.";
				break;
				
			case pCatTransm_picGuardar:
				pcMensaje = "Guarda la nueva transmisi�n o los cambios realizados a la informaci�n de una transmisi�n existente.";
				break;
				
			case pCatTransm_picCancelar:
				pcMensaje = "Cancela cualquier cambio en la informaci�n que se haya realizado para una transmisi�n.";
				break;
				
			case pCatTransm_picCerrar:
				pcMensaje = "Cierra el m�dulo de mantenimiento a transmisiones.";
				break;
				
			case pCatTransm_lstTransmisiones:
				pcMensaje = "Muestra la relaci�n de los nombres de transmisiones existentes en el sistema.";
				break;
				
			case pCatTransm_txtNombreTransmision:
				pcMensaje = "Campo que muestra o captura el nombre asignado a una transmisi�n.";
				break;
				
			case pCatTransm_numNumeroVelocidades:
				pcMensaje = "Indica el n�mero de velocidades disponibles en la transmisi�n seleccionada. El numero de velocidades no incluye la reversa";
				break;
				
			case pCatTransm_tblRelaciones:
				pcMensaje = "Contiene los valores de relaci�n de transmisi�n para cada una de las velocidades.";
				break;
				
			case pCatTransm_lblUbicacion:
				pcMensaje = "Indica el lado en que se encuentra ubicado el manipulador con respecto a la transmisi�n.";
				break;
			case pCatTransm_chkLadoDerecho:
				pcMensaje = "Indica el lado en que se encuentra ubicado el manipulador con respecto a la transmisi�n.";
				break;
			case pCatTransm_chkLadoIzquierdo:
				pcMensaje = "Indica el lado en que se encuentra ubicado el manipulador con respecto a la transmisi�n.";
				break;
		}
	}
	
	
	/*
	 * Mensajes para los elementos del panel iPanelEntrenamiento
	 */
	if (iPanel == iPanelEntrenamiento)
	{
		switch (iControl)
		{
			case pEntrenar_picNuevo:
				pcMensaje = "Permite ingresar una nueva configuraci�n de posiciones de velocidad para la transmisi�n.";
				break;
				
			case pEntrenar_picEliminar:
				pcMensaje = "Elimina la configuraci�n de velocidades para la transmisi�n seleccionada.";
				break;
				
			case pEntrenar_picCerrar:
				pcMensaje = "Cierra el m�dulo de entrenamiento.";
				break;
				
			case pEntrenar_lstTransmisiones:
				pcMensaje = "Muestra la relaci�n de los nombres de transmisiones existentes en el sistema.";
				break;
				
			case pEntrenar_canDistribucionVel:
				pcMensaje = "Muestra gr�ficamente la configuraci�n de velocidades para la transmisi�n actual.";
				break;
				
			case pEntrenar_tblEntrenamiento:
				pcMensaje = "Despliega los puntos que se han entrenado para la transmisi�n actual.";
				break;
		}
	}
	
	PRE_BarraEstado(strlen(pcMensaje)>0?pcMensaje:"");
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_BarraEstado
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Coloca el mensaje solicitado en la barra de mensajes
. Par�metro de entrada:	char *pcCadena | mensaje de texto que se colocar�
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_BarraEstado(char *pcCadena)
{
	SetCtrlVal (iPanelPrincipal, pPrincipal_txtEstado, pcCadena);
	return 0;
}

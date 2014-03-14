/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Archivo:					PRE_EventosCatTransmisiones.c
. Propósito:				Contiene las funciones referentes al proceso de 
.							eventos de transmisiones.
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		18 de Febrero de 2013
. Responsable modificación: 
.
*******************************************************************************/
#include "toolbox.h"
#include <userint.h>
#include "PRE_Principal.h"
#include "pre_variables.h"


int BDS_LeerDetalleTransmision(int iPanel, ...);
int BDS_LeerTransmisiones(int iPanel, int iControl);
stTransmisiones BDS_VerificaDescripcionTransmision(char  *pcNombre);
int PRE_CambioTransmision(int iPanel, int iControl);
int PRE_OpcionNuevaTransimision(void);
int PRE_ActualizaTablaRelaciones(void);
int PRE_CancelarProceso(void);
stTransmisiones PRE_OpcionActiva(stTransmisiones opcion);
int PRE_InsertarNuevaTransmision(void);
int PRE_ActualizarTransmision(void);
int PRE_GuardarTransmision(void);
char *GRA_IntStr(int iValor);
char *GRA_Strcat(int iNoElementos, ...);
stTransmisiones BDS_InsertaTransmision(char *pcDescripcion, int iNumVelocidades,
	double dListaRelaciones[], int iLadoManipulador);
stTransmisiones  BDS_ActualizaTransmision(int iId, int iNumVelocidades, 
		double dListaRelaciones[], int iLadoManipulador);
int PRE_SeleccionLadoManipulador(int iControl);
int PRE_EliminarTransmision(void);
char *BDS_InformeParaEliminar(int iId, char *pcDescripcion, int iNumVelocidades);
int PRE_EditarTransmision(void);
stTransmisiones BDS_EliminarTransmision(int iId);
int GRA_Items(int panel, int control);



/*****************************************************************************
.
. Función C:			PRE_PanelCatTransmisiones
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Captura los eventos registrados sobre el panel 
.						iPanelCatTransmisiones
. Parámetro de entrada:	los de una funcion callback
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
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
. Función C:			PRE_OpcionActiva
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Almacena e informa la opción del menu que se encuentra
.						activa en el sistema.
. Parámetro de entrada:	los de una funcion callback
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
stTransmisiones PRE_OpcionActiva(stTransmisiones opcion)
{
	static stTransmisiones Modo = TRA_MENU_PRINCIPAL;
	
	if (opcion != -1)
		Modo = opcion;

	return Modo;
}



/*****************************************************************************
.
. Función C:			PRE_SeleccionCatTransmisiones
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Captura los eventos registrados sobre los controles 
.						del panel iPanelCatTransmisiones
. Parámetro de entrada:	los de una funcion callback
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_SeleccionCatTransmisiones (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	if (event == EVENT_MOUSE_POINTER_MOVE)
	{
		PRE_ResaltarOpcion(panel, control, pCatTransm_cnvFondo, 7, pCatTransm_lstTransmisiones, 
			pCatTransm_numNumeroVelocidades, pCatTransm_txtNombreTransmision, pCatTransm_tblRelaciones,
			pCatTransm_lblUbicacion, pCatTransm_chkLadoDerecho, pCatTransm_chkLadoIzquierdo);
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
				
			case pCatTransm_picGuardar:
				PRE_GuardarTransmision();
				break;
				
			case pCatTransm_lstTransmisiones:
				PRE_CambioTransmision(panel, control);
				break;
				
			case pCatTransm_picEliminar:
				PRE_EliminarTransmision();
				break;
				
			case pCatTransm_picEditar:
				PRE_EditarTransmision();
				break;
		}
	}
	
	if (event == EVENT_VAL_CHANGED)
	{
		switch (control)
		{
			case pCatTransm_lstTransmisiones:
				PRE_CambioTransmision(panel, control);
				break;
				
			case pCatTransm_chkLadoDerecho:
				PRE_SeleccionLadoManipulador(pCatTransm_chkLadoDerecho);
				break;
				
			case pCatTransm_chkLadoIzquierdo:
				PRE_SeleccionLadoManipulador(pCatTransm_chkLadoIzquierdo);
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
. Función C:			PRE_IniciarCatTransmisiones
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Una vez que el usuario ha solicitado mostrar el 
.						catalogo de transmisiones, se despliega en pantalla
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	06 de Marzo de 2014
.
*****************************************************************************/
int PRE_IniciarCatTransmisiones()
{
	PRE_UbicarPanel(iPanelCatTransmisiones);
	
	if (BDS_LeerTransmisiones(iPanelCatTransmisiones, pCatTransm_lstTransmisiones) == TRA_CON_TRANSMISIONES)
	{
		PRE_CambioTransmision(iPanelCatTransmisiones, pCatTransm_lstTransmisiones);
		
		//habilita las funciones de editar y eliminar
		SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEditar, ATTR_DIMMED, 0);
		SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEliminar, ATTR_DIMMED, 0);
		SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picGuardar, ATTR_DIMMED, 1);
		SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCancelar, ATTR_DIMMED, 1);
	}
	else
	{
		//debe deshabilitar las funciones de editar, eliminar
		SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEditar, ATTR_DIMMED, 1);
		SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEliminar, ATTR_DIMMED, 1);
		SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picGuardar, ATTR_DIMMED, 1);
		SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCancelar, ATTR_DIMMED, 1);
	}
	
	PRE_OpcionActiva(-1);
	//PRE_ControlesEntrenamiento(Modo);
	//PRE_CondicionesIniciales();
	return 0;
}



/*****************************************************************************
.
. Función C:			PRE_CambioTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Procesa el cambio de selección de una transmisión en
.						la lista que muestra las que se han configurado.
.						Su efecto es actualizar la información en la pantalla
.						con la infomaci+ón en detalle de la mista.
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	06 de Marzo de 2014
.
*****************************************************************************/
int PRE_CambioTransmision(int iPanel, int iControl)
{
	int iId=0;	
	
	//verifica que tenga por lo menos un elemento
	if (GRA_Items(iPanelCatTransmisiones, pCatTransm_lstTransmisiones)>0)
	{
		GetCtrlVal(iPanelCatTransmisiones, pCatTransm_lstTransmisiones, &iId);
		BDS_LeerDetalleTransmision(iPanelCatTransmisiones, pCatTransm_txtNombreTransmision,
					pCatTransm_numNumeroVelocidades, pCatTransm_tblRelaciones, 
					pCatTransm_chkLadoIzquierdo, pCatTransm_chkLadoDerecho, iId);
	}
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_OpcionNuevaTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			El usuario ha solicitado capturar unan nueva 
.						transmision
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
int PRE_OpcionNuevaTransimision()
{
	PRE_OpcionActiva(TRA_AGREGAR_TRANSMISION);
	
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
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picGuardar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCancelar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelPrincipal, pPrincipal_btnMenu, ATTR_DIMMED, 1);
	
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_txtNombreTransmision, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_chkLadoDerecho, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_chkLadoIzquierdo, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_tblRelaciones, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_lstTransmisiones, ATTR_DIMMED,
					  1);
	
	
	
	PRE_ActualizaTablaRelaciones();
	
	return 0;
}



/*****************************************************************************
.
. Función C:			PRE_CancelarProceso
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			El usuario ha solicitado cancelar el proceso
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
int PRE_CancelarProceso()
{
	PRE_OpcionActiva(TRA_MENU_PRINCIPAL);
	
	//restablece los controles al comportamiento incial
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picAgregar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEditar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEliminar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCerrar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCancelar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelPrincipal, pPrincipal_btnMenu, ATTR_DIMMED, 0);
	
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_txtNombreTransmision, ATTR_CTRL_MODE,
					  VAL_INDICATOR);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_tblRelaciones, ATTR_CTRL_MODE,
					  VAL_INDICATOR);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, ATTR_CTRL_MODE,
					  VAL_INDICATOR);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_lstTransmisiones, ATTR_DIMMED,
					  0);
	
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_chkLadoDerecho, ATTR_CTRL_MODE,
					  VAL_INDICATOR);
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_chkLadoIzquierdo, ATTR_CTRL_MODE,
					  VAL_INDICATOR);
	
	PRE_IniciarCatTransmisiones();
	
	
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_ActualizaTablaRelaciones
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Cuando el usuario define un numero de velocodades,
.						se crean el número de renglones correspondientes en la
.						tabla de relaciones. Se tiene cuidado cuando el usuario
.						modifica a un valor menor y previamente existen ya
.						renglones creados.
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
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
	
	//vefifica si se necesita aplicar alguna validación antes de crear los 
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
			InsertTableRows (iPanelCatTransmisiones, pCatTransm_tblRelaciones, iRenglonesActuales+1,
						 iRenglonesUsuario-iRenglonesActuales, VAL_USE_MASTER_CELL_TYPE);
		}
		else
		{
			if (iRenglonesActuales != iRenglonesUsuario)
			{
				//se ha determinado que se deben eliminar renglones de la tabla
				//de relaciones
				DeleteTableRows (iPanelCatTransmisiones, pCatTransm_tblRelaciones, iRenglonesUsuario+1,
								 iRenglonesActuales-iRenglonesUsuario);
			}
		}
	}
	return 0;
}



/*****************************************************************************
.
. Función C:			PRE_GuardarTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Ejecuta la petición del usuario para almacenar la
.						información de la transmisión. Este proceso tiene 2
.						variantes, insertar una nueva transmision o el de
.						actualizar la información de una transmisión que ya
.						exista en el sistema. Esto lo determina el sistema
.						de manera automatica con la información de la opcion
.						que se encuentra activa en el sistema.
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
int PRE_GuardarTransmision()
{
	if (PRE_OpcionActiva(-1) == TRA_AGREGAR_TRANSMISION)
		PRE_InsertarNuevaTransmision();
	else
		PRE_ActualizarTransmision();
	
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_InsertarNuevaTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Inserta una nueva transmision en el sistema
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
int PRE_InsertarNuevaTransmision()
{
	char cMensajeError[500]={0};	//mensaje de error que se haya detectado
	char cDescripcion[250]={0};		//nombre asignado a la transmision
	int iNumVelocidades=0;			//numero de velocidades asociadas
	double dRelacion=0;				//valor de la relacion de velocidad
	char cVelocidad[5]={0};			//numero de velocidad en cadena
	int iLadoManipulador=0;			//indica el lado del manipulador (0 izq 1 der)
	double *pdListaRelaciones;		//lista de relaciones configuradas
	
	//se requeren implementar algunas validaciones antes de iniciar
	//el proceso de guardado de la nueva transmision
	
	//se verifica que la información se encuentra al 100% requisitada
	GetCtrlVal (iPanelCatTransmisiones, pCatTransm_txtNombreTransmision, cDescripcion);
	if (strlen(cDescripcion) == 0)
		strcat(cMensajeError,"- No se ha especificado un nombre a la transmision. \n");

	//verifica la informacion del numero de velocidades asocidadas
	GetCtrlVal (iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, &iNumVelocidades);
	if (iNumVelocidades == 1)
		strcat(cMensajeError,"- Se ha dejado el valor por defecto de 1 velocidad. \n");
	
	//vefifica los valores de relación para las velocidades capturadas
	pdListaRelaciones = malloc(sizeof(double)*iNumVelocidades);
	for (int i=0; i<iNumVelocidades; i++)
	{
		GetTableCellVal (iPanelCatTransmisiones, pCatTransm_tblRelaciones,
						 MakePoint(1,i+1), &dRelacion);
		pdListaRelaciones[i]=dRelacion;
		
		if (dRelacion == 0)
		{
			strcat(cMensajeError,
				   GRA_Strcat(3,"- Para la velocidad ",GRA_IntStr(i+1)," falta indicar la relación. \n"));
		}
	}
	
	//debe verificar que el nombre de la transmision no exista ya en el sistema
	if (BDS_VerificaDescripcionTransmision(cDescripcion)== TRA_EXISTE)
		strcat(cMensajeError,"- Ya existe en el sistema una transmisión con el mismo nombre. \n");
	
	GetCtrlVal (iPanelCatTransmisiones, pCatTransm_chkLadoIzquierdo, &iLadoManipulador);
	
	if (strlen(cMensajeError)>0)
	{
		MessagePopup ("Validación de información.", cMensajeError);
	}
	else
	{
		//inicia el proceso de guardado de la informacion, para lo cual obtiene
		//la lista de relaciones de las velocidades que se han configurado
		BDS_InsertaTransmision(cDescripcion, iNumVelocidades, pdListaRelaciones, iLadoManipulador);
		
		MessagePopup ("Registro guardado",
					  "Se almacenó la información correctamente.");
		
		//regresa la interfaz al modo menu principal
		PRE_CancelarProceso();
	}
	
	free(pdListaRelaciones);
	
	return 0;
}



/*****************************************************************************
.						
. Función C:			PRE_ActualizarTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Actualiza la informacion de la transmision que se
.						ha editado en el sistema
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
int PRE_ActualizarTransmision()
{
	char cMensajeError[500]={0};	//mensaje de error que se haya detectado
	char cDescripcion[250]={0};		//nombre asignado a la transmision
	int iNumVelocidades=0;			//numero de velocidades asociadas
	double dRelacion=0;				//valor de la relacion de velocidad
	char cVelocidad[5]={0};			//numero de velocidad en cadena
	int iLadoManipulador=0;			//indica el lado del manipulador (0 izq 1 der)
	int iId;						//identificador de la transmision
	double *pdListaRelaciones;		//lista de relaciones configuradas
	
	//se requeren implementar algunas validaciones antes de iniciar
	//el proceso de actualización de la transmision
	
	//obtiene el id de la transmision
	GetCtrlVal(iPanelCatTransmisiones, pCatTransm_lstTransmisiones, &iId);
	
	//verifica la informacion del numero de velocidades asocidadas
	GetCtrlVal (iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, &iNumVelocidades);
	if (iNumVelocidades == 1)
		strcat(cMensajeError,"- Se ha dejado el valor por defecto de 1 velocidad. \n");
	
	//vefifica los valores de relación para las velocidades capturadas
	pdListaRelaciones = malloc(sizeof(double)*iNumVelocidades);
	for (int i=0; i<iNumVelocidades; i++)
	{
		GetTableCellVal (iPanelCatTransmisiones, pCatTransm_tblRelaciones,
						 MakePoint(1,i+1), &dRelacion);
		pdListaRelaciones[i]=dRelacion;
		
		if (dRelacion == 0)
		{
			strcat(cMensajeError,
				   GRA_Strcat(3,"- Para la velocidad ",GRA_IntStr(i+1)," falta indicar la relación. \n"));
		}
	}
	
	GetCtrlVal (iPanelCatTransmisiones, pCatTransm_chkLadoIzquierdo, &iLadoManipulador);
	
	if (strlen(cMensajeError)>0)
	{
		MessagePopup ("Validación de información.", cMensajeError);
	}
	else
	{
		//inicia el proceso de guardado de la informacion, para lo cual obtiene
		//la lista de relaciones de las velocidades que se han configurado
		BDS_ActualizaTransmision(iId, iNumVelocidades, pdListaRelaciones, iLadoManipulador);
		
		MessagePopup ("Registro actualizado",
					  "Se ha actualizado la información correctamente.");
		
		//regresa la interfaz al modo menu principal
		PRE_CancelarProceso();
	}
	
	free(pdListaRelaciones);
	
	return 0;
}


/*****************************************************************************
.						
. Función C:			PRE_SeleccionLadoManipulador
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Procesa la selección del usuario para el lado en
.						que se encontrará ubicado el manipulador para la
.						transmision que se esta configurando
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	12 de Marzo de 2014
.
*****************************************************************************/
int PRE_SeleccionLadoManipulador(int iControl)
{
	
	if (iControl == pCatTransm_chkLadoDerecho)
	{
		SetCtrlVal (iPanelCatTransmisiones, pCatTransm_chkLadoDerecho, 1);
		SetCtrlVal (iPanelCatTransmisiones, pCatTransm_chkLadoIzquierdo, 0);
	}
	else
	{
		SetCtrlVal (iPanelCatTransmisiones, pCatTransm_chkLadoDerecho, 0);
		SetCtrlVal (iPanelCatTransmisiones, pCatTransm_chkLadoIzquierdo, 1);
	}
	
	
	return 0;
}



/*****************************************************************************
.						
. Función C:			PRE_EliminarTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Procesa la selección de eliminar la transmision
.						que el usuario ha seleccionado en la lista de la
.						pantalla.
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	12 de Marzo de 2014
.
*****************************************************************************/
int PRE_EliminarTransmision()
{
	char cDescripcion[250]={0};
	int iNumVelocidades=0;
	int iId=0;
	//se debe tomar el id de la transmisión para hacer una investigación
	//sobre la información que se encuentra vinculada a esta transmision
	//se propone mostrar en pantalla un informe con el total de datos
	//que se encuentran relacionados, a fin de que el usuario pueda
	//tomar la mejor decision sobre eliminar o no el elemento
	
	//primero, informar el número de elementos de velocidades que se
	//estarán eliminando
	GetCtrlVal (iPanelCatTransmisiones, pCatTransm_lstTransmisiones, &iId);
	GetCtrlVal (iPanelCatTransmisiones, pCatTransm_txtNombreTransmision, cDescripcion);
	GetCtrlVal (iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, &iNumVelocidades);
	
	if (ConfirmPopup ("Eliminar transmisión.", 
					  BDS_InformeParaEliminar(iId, cDescripcion, iNumVelocidades))==1)
	{
		BDS_EliminarTransmision(iId);
		
		//limpia la información de los controles
		SetCtrlVal (iPanelCatTransmisiones, pCatTransm_txtNombreTransmision, "");
		SetCtrlVal (iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, 1);
		SetCtrlVal (iPanelCatTransmisiones, pCatTransm_chkLadoIzquierdo, 1);
		SetCtrlVal (iPanelCatTransmisiones, pCatTransm_chkLadoDerecho, 0);
		DeleteTableRows (iPanelCatTransmisiones, pCatTransm_tblRelaciones, 1,
						 -1);
		
		MessagePopup ("Transmision eliminada",
					  "Se ha eliminado la información correctamente.");
		
		//regresa la interfaz al modo menu principal
		PRE_CancelarProceso();
	}
	return 0;
}




/*****************************************************************************
.						
. Función C:			PRE_EditarTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Procesa la selección para editar el elemento que se
.						ha seleccionado en la pantalla.
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	13 de Marzo de 2014
.
*****************************************************************************/
int PRE_EditarTransmision()
{
	//modifica el estado de los controles para el modo edición
	PRE_OpcionActiva(TRA_EDITAR_TRANSMISION);
	
	//modifica el comportamiento de los controles
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picAgregar, ATTR_DIMMED, 1);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEditar, ATTR_DIMMED, 1);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picEliminar, ATTR_DIMMED, 1);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCerrar, ATTR_DIMMED, 1);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picGuardar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_picCancelar, ATTR_DIMMED, 0);
	SetCtrlAttribute(iPanelPrincipal, pPrincipal_btnMenu, ATTR_DIMMED, 1);
	
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_txtNombreTransmision, ATTR_CTRL_MODE,
					  VAL_INDICATOR);
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_chkLadoDerecho, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute (iPanelCatTransmisiones,
					  pCatTransm_chkLadoIzquierdo, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_tblRelaciones, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_numNumeroVelocidades, ATTR_CTRL_MODE,
					  VAL_HOT);
	SetCtrlAttribute(iPanelCatTransmisiones, pCatTransm_lstTransmisiones, ATTR_DIMMED,
					  1);
	
	PRE_ActualizaTablaRelaciones();	
	
	return 0;
}




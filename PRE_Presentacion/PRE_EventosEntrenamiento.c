/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Archivo:					PRE_EventosEntremiento.c
. Prop�sito:				Contiene las funciones que antienden los diferentes
.							eventos del proceso de entrenamiento
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		18 de Febrero de 2013
. Responsable modificaci�n: 
.
*******************************************************************************/
#include <userint.h>
#include "PRE_Principal.h"
#include <formatio.h>
#include "toolbox.h"
#include "pre_variables.h"
#include "ent_funciones.h"


//prototipo de funciones
char *GRA_Strcat(int iNoElementos, ...);
int CAN_InfoControl(int iPanel, int iControl);
int CAN_DibujarMarca(int x, int y, int color);
int CAN_DibujarEtiqueta(int x, int y, char *pcCadena, int color);
int MOT_PosicionActual(long *iRotacional, long *iLineal);
char *GRA_InsertarPunto(int x, int y, char *pcEtiqueta);
ListType GRA_Dibujar(void);
ListType GRA_Etiquetas(void);
void GRA_Invertir(Point *p, int iSentido);
void GRA_Rotar(Point *p, int iRotar);
int GRA_InicializarCanvas(int iAlto, int iAncho, int iAltoC, int iAnchoC);
int GRA_ActualizaEtiqueta(int iPosicion, char *pcCadena);
int GRA_PanelModal(int iPanel);
int GRA_Dimmed(int iControl, int iModo);
int GRA_Visible(int iControl, int iModo);


int MOT_PosicionMotores(stMotores Modo);
int MOT_EstadoMotores(stMotores Modo);
int PAT_ConfigurarTiemposRecorrido(stTipoRecorrido Tipo, double dTiempo, int iNumCiclos, char *cRuta);
int PAT_AsignaEstadoEjecucion(stEdoEjecucion Estado);
char *PAT_PosicionCercaDe(double dCuentas, ListType Lista, int iPosLineal, 
	int iPosRotacional);
int COM_DriverHabilitado(void);
int MOT_MotorABB(stMotores Modo);
int MOT_VelocidadABB(double dVelocidadComando);
int MOT_SentidoGiroABB(stMotores Modo);
int ENT_ModoMotorABB(int iModo);

//variables globales
char cLetra='A';
int iAltoC=0;
int iAnchoC=0;
int esperaBoton=0;
int funcionPasoAPaso=0;	//por default es paso a paso
int MotorActivadoABB=0;

//variables para eliminar
long posRotActual=0;
long posLinActual=0;
long minimoRotacional=0;
long minimoLineal=0;
long maximoRotacional=0;
long maximoLineal=0;
double dRPMAbb=0.0;


/*****************************************************************************
.
. Funci�n C:			PRE_PanelEntrenamiento
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura los eventos registrados sobre los controles
.						del panel iPanelEntrenamiento relacionados con cerrar
.						la pantalla
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_PanelEntrenamiento (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			PRE_AnimacionMenu(OCULTAR);
			break;
			
		case EVENT_LEFT_CLICK_UP:
			PRE_AnimacionMenu(OCULTAR);
			break;
	}
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_DetallePosiciones
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Imprime en pantalla el detalle de la posici�n actual
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_DetallePosiciones()
{
	stPosicion p;
	int iNumElementos=0;
	
	
	iNumElementos = ListNumItems(miLista);
		
	
	for (int i=0;i<iNumElementos;i++)
	{
		ListGetItem(miLista, &p, i+1);
		printf("Posicion[%s]  --  Rotacional[%d]  -- Lineal[%d]  \n",p.cDescripcion, p.iRotacional, p.iLineal);
	}
	
	GetKey ();
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			PRE_SeleccionEntrenamiento
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura los eventos registrados sobre los controles
.						que integran la interfaz de entrenamiento
. Par�metro de entrada:	los de una funcion callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_SeleccionEntrenamiento (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	if (event == EVENT_MOUSE_POINTER_MOVE)
	{
		PRE_ResaltarOpcion(panel, control, pEntrenar_cnvFondo, 3, pEntrenar_lstTransmisiones, pEntrenar_canDistribucionVel, pEntrenar_tblEntrenamiento);
	}
	
	if (event == EVENT_LEFT_CLICK_UP)
	{
		switch (control)
		{
			case pEntrenar_picNuevo:
				PRE_ControlesEntrenamiento(CTRL_NUEVO_ENTRENAMIENTO);
				break;
				
			case pEntrenar_picInciarEntrenamient:
				ENT_EntrenamientoPaso1();
				break;
				
			case pEntrenar_picCancelaEntrenamien:
				ENT_AccionesDetenerEntrenamiento();
				PRE_IniciarEntrenamiento();
				PRE_Desmarcar(pEntrenar_cnvFondo);
				break;
				
			case pEntrenar_picAceptarConfiguraci:
				PRE_ControlesEntrenamiento(CTRL_ACEPTA_CONFIGURACION);
				break;
				
			case pEntrenar_picPruebaRecorrido:
				ENT_PruebaRecorrido();
				//ENT_DetallePosiciones();
				break;
				
			case pEntrenar_picDetenerPrueba:
				PAT_AsignaEstadoEjecucion(DESHABILITAR_MOVIMIENTO);
				MOT_EstadoMotores(MOT_DESHABILITAR);
				ENT_AsignaEstadoEntrenamiento(SIN_RECORRIDO);
				PRE_ControlesEntrenamiento(CTRL_RECORRIDO_FINALIZADO);
				ENT_ModoMotorABB(1);
				esperaBoton=1;
				break;
				
			case pEntrenar_picDescartarPuntos:
				ENT_AccionesDetenerEntrenamiento();
				PRE_IniciarEntrenamiento();
				PRE_Desmarcar(pEntrenar_cnvFondo);
				break;
			
			case pEntrenar_picCerrar:
				PRE_OcultarPanel(panel);
				break;
		}
	}
	
	if (event == EVENT_VAL_CHANGED)
	{
		switch (control)
		{
			case pEntrenar_lstTransmisiones:
				ENT_CargarPosiciones(iPanelEntrenamiento, pEntrenar_lstTransmisiones, pEntrenar_canDistribucionVel);
				break;
		}
	}
	
	if (event == EVENT_COMMIT)
	{
		switch (control)
		{
			case pEntrenar_tblEntrenamiento:
				ENT_ActualizarTabla(eventData1, eventData2);
				PRE_ControlesEntrenamiento(CTRL_ENTRENAMIENTO_TERMINADO);
				break;
		}
	}
	return 0;
	
}



/*****************************************************************************
.
. Funci�n C:			PRE_ControlesEntrenamiento
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Define el aspecto de la pantalla para cada caso o
.						funci�n en la que se encuentra el usuario dentro del
.						proceso de entrenamiento.
. Par�metro de entrada:	stEntrenar Modo | Indica el modo que se desea habilitar
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_ControlesEntrenamiento(stEntrenar Modo)
{
	switch (Modo)
	{
		case CTRL_CON_TRANSMISIONES:
			
			//debe habilitar la interfaz para el caso que existan transmisiones y ocultar el resto
			GRA_PanelModal(iPanelEntrenamiento);
			
			//habilita botones del menu de entrenamiento y la lista de transmisiones
			GRA_Dimmed(pEntrenar_picNuevo,FALSE);
			GRA_Dimmed(pEntrenar_picEliminar,FALSE);
			GRA_Dimmed(pEntrenar_picCerrar,FALSE);
			GRA_Dimmed(pEntrenar_lstTransmisiones,FALSE);
			
			//oculta botones de entrenamiento
			GRA_Visible(pEntrenar_picInciarEntrenamient, FALSE);
			GRA_Visible(pEntrenar_picCancelaEntrenamien, FALSE);
			
			//oculta botones de configuracion de puntos y tabla
			GRA_Visible(pEntrenar_picAceptarConfiguraci, FALSE);
			GRA_Visible(pEntrenar_picPruebaRecorrido, FALSE);
			GRA_Visible(pEntrenar_picDetenerPrueba, FALSE);
			GRA_Visible(pEntrenar_picAceptarPuntos, FALSE);
			GRA_Visible(pEntrenar_picDescartarPuntos, FALSE);
			GRA_Visible(pEntrenar_tblEntrenamiento, FALSE);
			
			//oculta controles de ayuda
			GRA_Visible(pEntrenar_txtAyuda, FALSE);
			GRA_Visible(pEntrenar_txtTituloAyuda, FALSE);
			GRA_Visible(pEntrenar_picVelocidades, FALSE);
			
			//restablece el boton del menu
			GRA_PanelModal(iPanelMenuPrincipal);
			GRA_Dimmed(pPrincipal_btnMenu, FALSE);
			break;
			
		case CTRL_SIN_TRANSMISIONES:
			//debe habilitar la interfaz para el caso que existan transmisiones y ocultar el resto
			GRA_PanelModal(iPanelEntrenamiento);
			
			//habilita botones del menu de entrenamiento y la lista de transmisiones
			GRA_Dimmed(pEntrenar_picNuevo, TRUE);
			GRA_Dimmed(pEntrenar_picEliminar, TRUE);
			GRA_Dimmed(pEntrenar_picCerrar,FALSE);
			GRA_Dimmed(pEntrenar_lstTransmisiones,FALSE);
			
			//oculta botones de entrenamiento
			GRA_Visible(pEntrenar_picInciarEntrenamient, FALSE);
			GRA_Visible(pEntrenar_picCancelaEntrenamien, FALSE);
			
			//oculta botones de configuracion de puntos y tabla
			GRA_Visible(pEntrenar_picAceptarConfiguraci, FALSE);
			GRA_Visible(pEntrenar_picPruebaRecorrido, FALSE);
			GRA_Visible(pEntrenar_picDetenerPrueba, FALSE);
			GRA_Visible(pEntrenar_picAceptarPuntos, FALSE);
			GRA_Visible(pEntrenar_picDescartarPuntos, FALSE);
			GRA_Visible(pEntrenar_tblEntrenamiento, FALSE);
			
			//oculta controles de ayuda
			GRA_Visible(pEntrenar_txtAyuda, FALSE);
			GRA_Visible(pEntrenar_txtTituloAyuda, FALSE);
			GRA_Visible(pEntrenar_picVelocidades, FALSE);
			
			//restablece el boton del menu
			GRA_PanelModal(iPanelMenuPrincipal);
			GRA_Dimmed(pPrincipal_btnMenu, FALSE);
			break;
			
		case  CTRL_NUEVO_ENTRENAMIENTO:
			
			GRA_PanelModal(iPanelEntrenamiento);
		
			//DEShabilita botones del menu de entrenamiento y la lista de transmisiones
			GRA_Dimmed(pEntrenar_picNuevo,TRUE);
			GRA_Dimmed(pEntrenar_picEliminar,TRUE);
			GRA_Dimmed(pEntrenar_picCerrar,TRUE);
			GRA_Dimmed(pEntrenar_lstTransmisiones,TRUE);
			
			//muestra los botones de entrenamiento
			GRA_Visible(pEntrenar_picInciarEntrenamient, TRUE);
			GRA_Visible(pEntrenar_picCancelaEntrenamien, TRUE);
			
			//elimina la funcion deshabilitada de los botones de entrenamiento
			GRA_Dimmed(pEntrenar_picInciarEntrenamient, FALSE);
			GRA_Dimmed(pEntrenar_picCancelaEntrenamien, FALSE);
			
			//bloquea el boton del menu
			GRA_PanelModal(iPanelMenuPrincipal);
			GRA_Dimmed(pPrincipal_btnMenu, TRUE);
			break;
			
		case CTRL_INICIA_CONFIGURACION:
			GRA_PanelModal(iPanelEntrenamiento);
			
			//actualiza el estado de los botones de entrenamiento
			GRA_Dimmed(pEntrenar_picInciarEntrenamient, TRUE);
			
			//muestra los controles de ayuda
			GRA_Visible(pEntrenar_txtAyuda, TRUE);
			GRA_Visible(pEntrenar_txtTituloAyuda, TRUE);
			
			ProcessDrawEvents();
			
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtTituloAyuda, "Inicializando proceso de entrenamiento");
			ResetTextBox (iPanelEntrenamiento, pEntrenar_txtAyuda, "");
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtAyuda, 
				GRA_Strcat(2,
					"\n",
					"La inicializaci�n de las rutinas de entrenamiento tomar� unos segundos, espere por favor..."));
			break;
			
		case CTRL_INICIA_ENTRENAMIENTO:
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtTituloAyuda, "Establecer referencia");
			ResetTextBox (iPanelEntrenamiento, pEntrenar_txtAyuda, "");
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtAyuda, 
				GRA_Strcat(4,
					"\n",
					"Como primer paso, debe acoplar el sistema manipulador a la palanca de velocidades.\n\n",
					"Posteriormente, en forma manual deber� posicionar la palanca de velocidades en la velocidad n�mero 1.\n\n",
					"Finalmente, para que el sistema registre esta posici�n como referencia, debe pulsar el Bot�n N�mero 2."));
			break;
			
			
		case CTRL_ENTRENAMIENTO_POSICION_CERO:
			GRA_PanelModal(iPanelEntrenamiento);
			GRA_Visible(pEntrenar_picVelocidades, TRUE);
			
			ResetTextBox (iPanelEntrenamiento, pEntrenar_txtAyuda, "");
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtTituloAyuda, "Entrenando posiciones");
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtAyuda, 
				GRA_Strcat(6,
					"\n",
					"Ahora comenzar� a entrenar cada una de las posiciones posibles en la transmisi�n. Para ello ",
					"en forma manual estar� modificando la posici�n de la palanca de velocidades y en cada una de ",
					"las posiciones de inter�s (ver imagen de ejemplo), pulsar� el Bot�n N�mero 3 para que ",
					"el sistema registre la posici�n.\n\n",
					"Para finalizar el entrenamiento pulse el Bot�n N�mero 1"));
			break;
			
		case  CTRL_ENTRENAMIENTO_TERMINADO:
			GRA_PanelModal(iPanelEntrenamiento);
			
			//oculta controles de ayuda
			GRA_Visible(pEntrenar_txtAyuda, FALSE);
			GRA_Visible(pEntrenar_txtTituloAyuda, FALSE);
			GRA_Visible(pEntrenar_picVelocidades, FALSE);
			
			GRA_Dimmed(pEntrenar_picCancelaEntrenamien, TRUE);
			
			
			//oculta botones de configuracion de puntos y tabla
			GRA_Visible(pEntrenar_picAceptarConfiguraci, TRUE);
			GRA_Visible(pEntrenar_picPruebaRecorrido, TRUE);
			GRA_Visible(pEntrenar_picDetenerPrueba, TRUE);
			GRA_Visible(pEntrenar_picAceptarPuntos, TRUE);
			GRA_Visible(pEntrenar_picDescartarPuntos, TRUE);
			GRA_Visible(pEntrenar_tblEntrenamiento, TRUE);
			
			GRA_Dimmed(pEntrenar_picAceptarConfiguraci, FALSE);
			GRA_Dimmed(pEntrenar_picPruebaRecorrido, FALSE);		//<<<<-------
			GRA_Dimmed(pEntrenar_picDetenerPrueba, TRUE);
			GRA_Dimmed(pEntrenar_picAceptarPuntos, TRUE);
			GRA_Dimmed(pEntrenar_picDescartarPuntos, FALSE);
			GRA_Dimmed(pEntrenar_picCancelaEntrenamien, TRUE);
			
			break;
			
		case CTRL_ACEPTA_CONFIGURACION:
			GRA_PanelModal(iPanelEntrenamiento);
			
			GRA_Dimmed(pEntrenar_picAceptarConfiguraci, TRUE);
			GRA_Dimmed(pEntrenar_picPruebaRecorrido, FALSE);
			GRA_Dimmed(pEntrenar_picAceptarPuntos, FALSE);
			break;
		
		case CTRL_ESPERA_BOTON_INICIO_PRUEBA:
			GRA_PanelModal(iPanelEntrenamiento);
			
			GRA_Visible(pEntrenar_tblEntrenamiento, FALSE);
			
			GRA_Visible(pEntrenar_txtAyuda, TRUE);
			GRA_Visible(pEntrenar_txtTituloAyuda, TRUE);
			
			GRA_Dimmed(pEntrenar_picPruebaRecorrido, TRUE);
			GRA_Dimmed(pEntrenar_picDetenerPrueba, FALSE);
			
			GRA_Dimmed(pEntrenar_picAceptarPuntos, TRUE);
			GRA_Dimmed(pEntrenar_picDescartarPuntos, TRUE);
			
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtTituloAyuda, "Iniciar recorrido de prueba");
			ResetTextBox (iPanelEntrenamiento, pEntrenar_txtAyuda, "");
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtAyuda, 
				GRA_Strcat(2,
					"\n",
					"Para dar inicio al recorrido de prueba, pulse el Bot�n N�mero 2."));
			break;
			
		case CTRL_IR_POSICION_INICIAL:
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtTituloAyuda, "Localizando posici�n de inicio");
			ResetTextBox (iPanelEntrenamiento, pEntrenar_txtAyuda, "");
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtAyuda, 
				GRA_Strcat(2,
					"\n",
					"Espere un momento, se est� colocando la palanca de velocidades en la posici�n inicial..."));
			break;
			
		
		case CTRL_RECORRIDO_PRUEBA:
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtTituloAyuda, "Ejecutando recorrido de prueba");
			ResetTextBox (iPanelEntrenamiento, pEntrenar_txtAyuda, "");
			SetCtrlVal(iPanelEntrenamiento, pEntrenar_txtAyuda, 
				GRA_Strcat(3,
					"\n",
					"Se est� ejecutando el recorrido de prueba por cada uno de los puntos que fueron entrenados.\n",
					"En la pantalla se estar� resaltando en color verde cada una de las posiciones que el sistema ha alcanzado."));
			break;
			
		case CTRL_RECORRIDO_FINALIZADO:
			GRA_PanelModal(iPanelEntrenamiento);
			
			GRA_Visible(pEntrenar_txtAyuda, FALSE);
			GRA_Visible(pEntrenar_txtTituloAyuda, FALSE);
			
			GRA_Visible(pEntrenar_tblEntrenamiento, TRUE);
			
			GRA_Dimmed(pEntrenar_picPruebaRecorrido, FALSE);
			GRA_Dimmed(pEntrenar_picDetenerPrueba, TRUE);
			
			GRA_Dimmed(pEntrenar_picAceptarPuntos, FALSE);
			GRA_Dimmed(pEntrenar_picDescartarPuntos, FALSE);
			
			break;
	}
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_CondicionesIniciales
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Establece las condiciones iniciales al momento de 
.						ingresar a la modalidad de entrenamiento.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_CondicionesIniciales()
{
	int iAlto=0;
	int iAncho=0;
	
	CAN_InfoControl(iPanelEntrenamiento, pEntrenar_canDistribucionVel);
	
	if (miLista != NULL)
		ListDispose(miLista);
	
	miLista = ListCreate (sizeof(stPosicion));
	cLetra='A';
	
	GetCtrlAttribute(iPanelEntrenamiento, pEntrenar_canDistribucionVel, ATTR_WIDTH, &iAnchoC);
	GetCtrlAttribute(iPanelEntrenamiento, pEntrenar_canDistribucionVel, ATTR_HEIGHT, &iAltoC);
	
	GRA_InicializarCanvas(iAltoC,iAnchoC, ALTO_LINEAL, ANCHO_ROTACIONAL);
	
	CanvasClear (iPanelEntrenamiento, pEntrenar_canDistribucionVel, VAL_ENTIRE_OBJECT);
	
	ENT_InformacionTabla(INICIALIZAR);
	
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			PRE_IniciarEntrenamiento
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Una vez que el usuario ha solicitado iniciar un nuevo
.						entrenamiento, se deben establecer algunas condiciones
.						basicas de inicio.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_IniciarEntrenamiento()
{
	stEntrenar Modo = CTRL_SIN_TRANSMISIONES;
	
	PRE_UbicarPanel(iPanelEntrenamiento);
	
	Modo = ENT_CargarTransmisiones(iPanelEntrenamiento, pEntrenar_lstTransmisiones);
	
	if (Modo == CTRL_CON_TRANSMISIONES)
		ENT_CargarPosiciones(iPanelEntrenamiento, pEntrenar_lstTransmisiones, pEntrenar_canDistribucionVel);
	
	PRE_ControlesEntrenamiento(Modo);
	PRE_CondicionesIniciales();
	return 0;
}




/*****************************************************************************
.
. Funci�n C:			PRE_EntrenamientiPaso1
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Establece las condiciones iniciales al momento de 
.						ingresar a la modalidad de entrenamiento.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_EntrenamientoPaso1()
{
	PRE_ControlesEntrenamiento(CTRL_INICIA_CONFIGURACION);
	ENT_AccionesIniciarEntrenamiento(&ENT_EntrenamientoPaso2);
	PRE_ControlesEntrenamiento(CTRL_INICIA_ENTRENAMIENTO);
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_TipoSeleccionado
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Determina del tipo de posici�n de la palanca
.						VELOCIDAD, NEUTRAL, REVERSA
. Par�metro de entrada:	char *pcCadena | nombre de la posici�n
. Par�metro de salida:	stTipo | tipo de posici�n
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
stTipo ENT_TipoSeleccionado(char *pcCadena)
{
	stTipo iTipo=VELOCIDAD;
	
	if (CompareStrings(pcCadena,0,"Velocidad",0,0)==0)
		iTipo=VELOCIDAD;
	if (CompareStrings(pcCadena,0,"Neutral",0,0)==0)
		iTipo=NEUTRAL;
	if (CompareStrings(pcCadena,0,"Reversa",0,0)==0)
		iTipo=REVERSA;
	
	
	return iTipo;
}


/*****************************************************************************
.
. Funci�n C:			ENT_ActualizaDetalle
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Actualiza la informaci�n capturada en la tabla de
.						entrenamiento.
. Par�metro de entrada:	int iRenglon | posici�n del renglon
.						int iColumna | posici�n de la columna
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_ActualizaDetalle(int iRenglon, int iColumna)
{
	stPosicion p;
	char cDescripcion[20]={0};
	
	/*
	el renglon me indica que elemento de la lista vamos a modificar
	*/
	ListGetItem(miLista, &p, iRenglon);
	GetTableCellVal(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(3, iRenglon), cDescripcion);
	
	Fmt(p.cDescripcion, "%s<%s", cDescripcion);
	
	ListReplaceItem (miLista, &p, iRenglon);
	GRA_ActualizaEtiqueta(iRenglon, cDescripcion);
	
	ENT_PintarPuntos(CONFIGURANDO, p);
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_CargaDetalle
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Actualiza la informaci�n en la tabla de entrenamiento
.						con la informaci�n disponible en memoria.
. Par�metro de entrada:	int iRenglon | posici�n del renglon
.						int iColumna | posici�n de la columna
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_CargaDetalle(int iRenglon, int iColumna)
{
	char cTipo[20]={0};
	stTipo Tipo=VELOCIDAD;;
	
	GetTableCellVal(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(2, iRenglon), cTipo);
	Tipo = ENT_TipoSeleccionado(cTipo); 
	DeleteTableCellRingItems(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(3, iRenglon), 0, -1);
	
	switch(Tipo)
	{
		case VELOCIDAD:
			for (int i=0;i<TOTAL_VELOCIDADES;i++)
			{
				char cNumero[10]={0};
			
				Fmt(cNumero,"%s<%d",i+1);
				InsertTableCellRingItem(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(3, iRenglon), -1, cNumero);
			}
			break;
		case NEUTRAL:
			for (int i=0;i<TOTAL_NEUTRALES;i++)
			{
				char cNumero[10]={0};
			
				Fmt(cNumero,"%s<%s%d","N",i+1);
				InsertTableCellRingItem(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(3, iRenglon), -1, cNumero);
			}
			break;
			
		case REVERSA:
			InsertTableCellRingItem(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(3, iRenglon), -1, "R");
			break;
	}
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_CargarTipos
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Actualiza la tabla con una lista que describe el tipo
.						de elemento de velocidad se trata
. Par�metro de entrada:	int iRenglon | posici�n del renglon
.						int iColumna | posici�n de la columna
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_CargarTipos(int iRenglon, int iColumna)
{
	DeleteTableCellRingItems(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(iColumna, iRenglon), 0, -1);
	
	InsertTableCellRingItem(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(iColumna, iRenglon), -1, "Velocidad");
	InsertTableCellRingItem(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(iColumna, iRenglon), -1, "Neutral");
	InsertTableCellRingItem(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(iColumna, iRenglon), -1, "Reversa");
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_ActualizarListaConTabla
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Actualiza la informaci�n de la lista con la informaci�n
.						que se encuentra en la tabla
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_ActualizarListaConTabla()
{
	int iNumRenglones=0;
	char cDescripcion[10]={0};
	int iPlano=0;
	stTipo Tipo=0;
	char cVar[10]={0};
	
	GetNumTableRows (iPanelEntrenamiento, pEntrenar_tblEntrenamiento, &iNumRenglones);
	
	for (int i=0; i<iNumRenglones; i++)
	{
		//obtiene el tipo
		GetTableCellVal(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(2, i+1), cVar);
		Tipo = ENT_TipoSeleccionado(cVar); 
		
		//obtiene la descripcion
		GetTableCellVal(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(3, i+1), cDescripcion);
		
		//Obtiene el plano
		GetTableCellVal(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(4, i+1), cVar);
		Fmt(&iPlano,"%d<%s",cVar);
		
		//ahora actualiza en la lista
		stPosicion p;
		
		ListGetItem(miLista, &p, i+1);
		
		p.iPlano = iPlano;
		p.iTipo = Tipo;
		Fmt(p.cDescripcion,"%s<%s",cDescripcion);
		
		ListReplaceItem(miLista, &p, i+1);
	}
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			PRE_GuardarPosiciones
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura el evento solicitado por el usuario, para el
.						guardado de la informaci�n entrenada
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_GuardarPosiciones (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	char miArchivo[260]={0};
	char cBuffer[1000]={0};
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			if (FileSelectPopupEx ("", "*.txt", "*.txt",
							   "Indica el nombre de archivo...", VAL_SAVE_BUTTON,
							   0, 0, miArchivo)!=0)
			{
				//verifica si existe el nombre del archivo
				if (FileExists (miArchivo, 0)==1)
				{
					DeleteFile (miArchivo);
				}
				
				//obtiene el numero de elementos de la lista
				int numero = ListNumItems (miLista);
				
				//ahora crea un arreglo con la informaci�n de la lista
				int pos[30][7]={0};
				
				//se inicializa la informaci�n del arreglo
				for (int i=0;i<30;i++)
				{
					pos[i][0]=-100;
					pos[i][1]=-100;
					pos[i][2]=-100;
					pos[i][3]=-100;
					pos[i][4]=-100;
					pos[i][5]=-100;
					pos[i][6]=-100;
				}
				
				//comienza a extraer la informaci�n de la lista y la coloca en el arreglo
				for (int i=0;i<numero;i++)
				{
					stPosicion p;
					
					ListGetItem (miLista, &p, i+1);
					pos[i][0]=p.iLineal;
					pos[i][1]=p.iNumeroVelocidad;
					pos[i][2]=p.iPlano;
					pos[i][3]=p.iRotacional;
					pos[i][4]=p.iTipo;
					pos[i][5]=p.x;
					pos[i][6]=p.y;
					strcat(cBuffer,p.cDescripcion);
					strcat(cBuffer,";");
				}
				
				ArrayToFile (miArchivo, pos, VAL_INTEGER, 210, 7,
							 VAL_DATA_MULTIPLEXED, VAL_GROUPS_AS_COLUMNS,
							 VAL_SEP_BY_COMMA, 10, VAL_ASCII, VAL_TRUNCATE);
				
				int iEtiqueta = OpenFile ("Etiquetas.txt", VAL_WRITE_ONLY, VAL_TRUNCATE,
									  VAL_ASCII);
				WriteFile (iEtiqueta, cBuffer, strlen(cBuffer));
				CloseFile (iEtiqueta);
			}
			break;
	}
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			PRE_LeerPosiciones
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Carga la informaci�n de un archivo existente
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PRE_LeerPosiciones (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	char miArchivo[260]={0};
	char cBuffer[1000]={0};
	char cEtiqueta[10]={0};
	
	switch (event)
	{
		case EVENT_COMMIT:

			FileSelectPopupEx ("", "*.txt", "*.txt",
							   "Selecciona el nombre de archivo...",
							   VAL_SELECT_BUTTON, 0, 0, miArchivo);
			
			//analiza el numero de elementos que se han guardado
			int pos[30][7]={0};
			
			FileToArray (miArchivo, pos, VAL_INTEGER, 210, 7,
						 VAL_DATA_MULTIPLEXED, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			
			//verifica que la lista se encuentre vacia
			int numero = ListNumItems (miLista);
			
			//comienza a insertar la informaci�n en la lista
			stPosicion p;
			int iIndice=0;
			int iIndice2=0;
			
			//lee la informacion de las etiquetas
			int iet = OpenFile ("Etiquetas.txt", VAL_READ_ONLY, VAL_TRUNCATE,
								  VAL_ASCII);
			ReadFile (iet, cBuffer, 1000);
			CloseFile (iet);
			
			for (int i=0;i<30;i++)
			{
				Fmt(cEtiqueta,"%s<%s","");
				
				//solo cuando los primeros elementos sean igual a -100 aplica
				if ((pos[i][0] == -100) && (pos[i][1] == -100) &&
						(pos[i][2] == -100) && (pos[i][3] == -100))
				{
					//no hacer nada
				}
				else
				{
					//localiza la siguiente etiqueta valida
					while (cBuffer[iIndice++]!=';');
					
					iIndice--;
					
					CopyString (cEtiqueta, 0, cBuffer, iIndice2, iIndice-iIndice2);
					
					iIndice++;
					iIndice2=iIndice;
					
					Fmt(p.cDescripcion,"%s<%s",cEtiqueta);
					p.iLineal         =pos[i][0];
					p.iNumeroVelocidad=pos[i][1];
					p.iPlano          =pos[i][2];
					p.iRotacional     =pos[i][3];
					p.iTipo           =pos[i][4];
					p.x               =pos[i][5];
					p.y               =pos[i][6];
					
					ListInsertItem (miLista, &p, END_OF_LIST);
					
					char cCadena[20]={0};
					
					Fmt(cCadena,"%s<%s",p.cDescripcion);
					cLetra++;
	
					GRA_InsertarPunto(p.iLineal, p.iRotacional,cCadena);
					ENT_PintarPuntos(ENTRENANDO);
					ENT_InformacionTabla(INSERTAR);
					ENT_PintarPuntos(CONFIGURANDO, p);
				}
			}
			break;
	}
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_ActualizarTabla
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Recibe como parametros el punto que se ha modificado
.						en la tabla y procede a actualizar la lista con la
.						nueva informaci�n.
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_ActualizarTabla(int iE1, int iE2)
{
	//cuando se ha cambiado el tipo
	if (iE2 == 2)
		ENT_CargaDetalle(iE1, iE2);
	
	//cuando se ha cambiado el detalle
	if (iE2 == 3)
		ENT_ActualizaDetalle(iE1, iE2);
	
	//si importar el cambio que se haya hecho, se debe actualizar la informaci�n de la tabla en la estructura
	ENT_ActualizarListaConTabla();
	
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_InformacionTabla
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_InformacionTabla(stTabla accion)
{
	static char cPosicion='A';
	static int iRenglon=0;
	char cCadena[5]={0};
	
	if (accion == INSERTAR)
	{
		Fmt(cCadena,"%s<%c",cPosicion);
		iRenglon++;
		
		InsertTableRows (iPanelEntrenamiento, pEntrenar_tblEntrenamiento, -1, 1,
						 VAL_USE_MASTER_CELL_TYPE);
		
		SetTableCellVal (iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint (1,iRenglon),
						 cCadena);
		
		ENT_CargarTipos(iRenglon, 2);
		
		ENT_CargaDetalle(iRenglon, 2);
		
		//crea la lista para el total de plano configurados por defecto
		for (int i=0;i<TOTAL_PLANOS;i++)
		{
			char cNumero[10]={0};
			
			Fmt(cNumero,"%s<%d",i+1);
			InsertTableCellRingItem(iPanelEntrenamiento, pEntrenar_tblEntrenamiento, MakePoint(4, iRenglon), -1, cNumero);
		}
		cPosicion++;
	}
	else
	{
		DeleteTableRows (iPanelEntrenamiento, pEntrenar_tblEntrenamiento, 1, -1);
		cPosicion='A';
		iRenglon=0;
	}
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_InformacionTabla
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_PintarPuntos(stPintaMarcas Tipo,...)
{
	va_list pa;
	ListType ListaTmp;
	ListType ListaTmp2;
	stPosicion p = MistPosicion;
	char *pcDescripcion=NULL;
	
	ListaTmp = GRA_Dibujar();
	ListaTmp2 = GRA_Etiquetas();
	
	int iNum = ListNumItems(ListaTmp);
	
	va_start(pa, Tipo);
	
	if (Tipo == CONFIGURANDO)
		p = va_arg(pa,stPosicion);
	
	if (Tipo == RECORRIDO)
		pcDescripcion = va_arg(pa,char*);
	
	//printf ("Numero = %d   \n", iNum);
	
	int iSentido=0;
	int iRotar=0;

	iSentido=1;
	iRotar=1;
		
	if (Tipo == ENTRENANDO)
		CanvasClear (iPanelEntrenamiento, pEntrenar_canDistribucionVel, VAL_ENTIRE_OBJECT);

	
	for (int i=0;i<iNum;i++)
	{
		Point punto;
		char cEtiqueta[40]={0};

		ListGetItem(ListaTmp, &punto, i+1);
		ListGetItem(ListaTmp2, &cEtiqueta, i+1); 
	
		GRA_Invertir(&punto, iSentido);
		GRA_Rotar(&punto,iRotar);
	
		switch (Tipo)
		{
			case ENTRENANDO:
				CAN_DibujarMarca(punto.x,punto.y, VAL_BLACK);
				CAN_DibujarEtiqueta(punto.x,punto.y,cEtiqueta, VAL_WHITE);
				break;
				
			case CONFIGURANDO:
				if (CompareStrings(p.cDescripcion,0,cEtiqueta,0,0)==0)
				{
					CAN_DibujarMarca(punto.x,punto.y, VAL_GREEN);
					CAN_DibujarEtiqueta(punto.x,punto.y,cEtiqueta, VAL_BLACK);
				}
				break;
				
			case RECORRIDO:
				if (CompareStrings(pcDescripcion,0,cEtiqueta,0,0)==0)
				{
					CAN_DibujarMarca(punto.x,punto.y, VAL_GREEN);
					CAN_DibujarEtiqueta(punto.x,punto.y,cEtiqueta, VAL_BLACK);
				}
				break;
		}
	}
	
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_CapturaNuevoPunto
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura el evento generado por el usuario desde la
.						botonera, en donde ha solicitado que la posici�n
.						actual del sistema, sea registrado como posici�n
.						a entrenar.
. Par�metro de entrada:	los de una funci�n callback
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_CapturarNuevoPunto()
{
	char cCadena[6]={0};
	int iCuentas=0;
	int iTmp=0;
	stPosicion miPosicionCaptura;
	
	GetCtrlAttribute(iPanelEntrenamiento, pEntrenar_canDistribucionVel, ATTR_WIDTH, &iTmp);
	iCuentas = ANCHO_ROTACIONAL;
	MOT_PosicionActual(&miPosicionCaptura.iRotacional, &miPosicionCaptura.iLineal);
	
	miPosicionCaptura.x = miPosicionCaptura.iLineal/(iCuentas/iTmp);
	
	
	GetCtrlAttribute(iPanelEntrenamiento, pEntrenar_canDistribucionVel, ATTR_HEIGHT, &iTmp);
	iCuentas = ALTO_LINEAL;
	
	miPosicionCaptura.y = miPosicionCaptura.iRotacional/(iCuentas/iTmp);
	
	ListInsertItem (miLista, &miPosicionCaptura, END_OF_LIST);
	
	Fmt(cCadena,"%s<%c",cLetra);
	cLetra++;
	
	GRA_InsertarPunto(miPosicionCaptura.iLineal, miPosicionCaptura.iRotacional,cCadena);
	
		
	ENT_PintarPuntos(ENTRENANDO);
	
	ENT_InformacionTabla(INSERTAR);
		
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_EntrenamientoPaso2
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Una vez que el usario ha establecido la posici�n
.						de cero (referencia), se procede a la habilitaci�n
.						para la captura del resto de las posiciones.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_EntrenamientoPaso2()
{
	//cancela la asociacion de funcion  anterior
	ENT_AccionesIniciarEntrenamiento(NULL);
	
	PRE_ControlesEntrenamiento(CTRL_ENTRENAMIENTO_POSICION_CERO);
	ENT_AccionesSegundoPasoEntrenamiento(&ENT_EntrenamientoPaso3, &ENT_CapturarNuevoPunto);
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_EntrenamientoPaso3
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura el evento en donde el usuario ha dicho que
.						el entrenamiento ha finalizado.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_EntrenamientoPaso3()
{
	//Cuando el usuario ha indicado que el entrenamiento ha finalizado
	PRE_ControlesEntrenamiento(CTRL_ENTRENAMIENTO_TERMINADO);
	ENT_AccionesSegundoPasoEntrenamiento(NULL, NULL);
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_CicloConcluido
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Esta funci�n opera como contador de ciclos.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_CicloConcluido()
{
	int iNum=0;
	GetCtrlVal (iPanelEntrenamiento, pEntrenar_numNumeroCiclosR, &iNum);
	iNum++;
	SetCtrlVal (iPanelEntrenamiento, pEntrenar_numNumeroCiclosR, iNum);

	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_ContinuarRecorridoPrueba
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Esta funci�n da inicio al proceso de movimiento
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_ContinuarRecorridoPrueba()
{
	double dTiempoTmp=0;
	int iNumCiclos=0;
	char cRutaCiclo[500]={0};
	
	//obtiene la velocidad indicada en la pantalla
	GetCtrlVal (iPanelEntrenamiento, pEntrenar_numTiempoMovimiento, &dTiempoTmp);
	GetCtrlVal (iPanelEntrenamiento, pEntrenar_numNumeroCiclos, &iNumCiclos);
	GetCtrlVal (iPanelEntrenamiento, pEntrenar_strRutaCiclo, cRutaCiclo);
	GetCtrlVal (iPanelEntrenamiento, pEntrenar_numRPMAbb, &dRPMAbb);
	SetCtrlVal (iPanelEntrenamiento, pEntrenar_numNumeroCiclosR, 0);
	
	PAT_AsignaEstadoEjecucion(HABILITAR_MOVIMIENTO);
	PAT_ConfigurarTiemposRecorrido(PRUEBA_RECORRIDO_INICIO, dTiempoTmp,iNumCiclos, cRutaCiclo);
	PRE_ControlesEntrenamiento(CTRL_RECORRIDO_PRUEBA);
	
	ENT_AsignaEstadoEntrenamiento(CON_RECORRIDO);
	PAT_ConfigurarTiemposRecorrido(PRUEBA_RECORRIDO_PUNTOS, dTiempoTmp,iNumCiclos, cRutaCiclo);
	PRE_ControlesEntrenamiento(CTRL_RECORRIDO_FINALIZADO);
	
	ENT_ConfirmaInicioRecorridoPrueba(NULL, NULL);
	
	ENT_AsignaEstadoEntrenamiento(SIN_RECORRIDO);
	PAT_AsignaEstadoEjecucion(DESHABILITAR_MOVIMIENTO);
	MOT_EstadoMotores(MOT_DESHABILITAR);
	
	ENT_PintarPuntos(ENTRENANDO);
	ENT_ModoMotorABB(1);
	esperaBoton=1;
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_ColocaMensaje
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Despliega un mensaje en la pantalla
. Par�metro de entrada:	char *cMensaje | cadena con el mensaje a desplegar
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_ColocaMensaje(char *cMensaje)
{
	InsertTextBoxLine (iPanelEntrenamiento, pEntrenar_txtCajaTexto, 0,
					   cMensaje);
	
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_ActualizaEsperaBoton
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura el evento de espera
. Par�metro de entrada:	char *cMensaje | cadena con el mensaje a desplegar
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_ActualizaEsperaBoton()
{
	esperaBoton = 1;
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_LeeEsperaBoton
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Funci�n utilizada para la ejecuci�n de movimientos
.						paso a paso.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_LeeEsperaBoton()
{
	
	SetCtrlVal(iPanelEntrenamiento, pEntrenar_strMensajeEspera, "Pulsa Boton 2 para continuar...");
	
	if (funcionPasoAPaso == 1)
	{
		esperaBoton=0;
		while (esperaBoton == 0)
		{
			DelayWithEventProcessing(0.5);
		}
	}
	SetCtrlVal(iPanelEntrenamiento, pEntrenar_strMensajeEspera, "Avanzando a siguiente posicion...");
	
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_FuncionPasoAPaso
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Implementa la habilitaci�n de la ejecuci�n paso a paso
.						al asociar la ejecuci�n de una funci�n cuando se
.						pulse el boton X de la botonera.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_FuncionPasoAPaso()
{
	funcionPasoAPaso = !funcionPasoAPaso;
	SetCtrlVal (iPanelEntrenamiento, pEntrenar_ledPasoAPaso, funcionPasoAPaso);
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_DetenerPruebaBotonera
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura la solicitud por parte del usuario de 
.						detener la ejecuci�n de la prue ba actual
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_DetenerPruebaBotonera()
{
	ENT_ConfirmaInicioRecorridoPrueba(NULL, NULL);
	PAT_AsignaEstadoEjecucion(DESHABILITAR_MOVIMIENTO);
	MOT_EstadoMotores(MOT_DESHABILITAR);
	ENT_AsignaEstadoEntrenamiento(SIN_RECORRIDO);
	PRE_ControlesEntrenamiento(CTRL_RECORRIDO_FINALIZADO);
	ENT_ModoMotorABB(1);
	esperaBoton=1;
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ENT_IniciarRecorridoPrueba
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Da inicio al proceso de prueba, enviando en primer
.						paso la palanca a la posici�n de inicio del recorrido.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_IniciarRecorridoPrueba()
{
	PRE_ControlesEntrenamiento(CTRL_IR_POSICION_INICIAL);
	ENT_CalculaDesplazamientoPrueba();

	MOT_PosicionMotores(MOT_POSICION_ACTUAL);
	ENT_TorqueMotores(5.24, 2.5);
	MOT_EstadoMotores(MOT_HABILITAR);
	
	PRI_LlamadasRemotas(&ENT_ContinuarRecorridoPrueba);
	ENT_SiguientePaso(&ENT_ActualizaEsperaBoton);
	
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ENT_ModoMotorABB
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Da inicio al proceso de prueba, enviando en primer
.						paso la palanca a la posici�n de inicio del recorrido.
. Par�metro de entrada:	int iModo | 0 deshabilitar
.									1 habilitar
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_ModoMotorABB(int iModo)
{
	if (iModo==0)
	{
		if (MotorActivadoABB == 0)
		{
			SetCtrlVal (iPanelEntrenamiento, pEntrenar_ledMotorABB, 1);
			MOT_MotorABB(MOT_ABB_HABILITAR);
			MOT_SentidoGiroABB(MOT_GIRO_HORARIO);
			MOT_VelocidadABB(dRPMAbb);
		}
		else
		{
			SetCtrlVal (iPanelEntrenamiento, pEntrenar_ledMotorABB, 0);
			MOT_MotorABB(MOT_ABB_DESHABILITAR);
		}
	
		MotorActivadoABB = !MotorActivadoABB;
	}
	else
	{
		SetCtrlVal (iPanelEntrenamiento, pEntrenar_ledMotorABB, 0);
		MotorActivadoABB=0;
		MOT_MotorABB(MOT_ABB_DESHABILITAR);
	}
	
	return 0;
}
	


/*****************************************************************************
.
. Funci�n C:			ENT_PruebaRecorrido
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Da inicio al proceso de ejecuci�n de prueba en el
.						recorrido.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ENT_PruebaRecorrido()
{
	ENT_PintarPuntos(ENTRENANDO);
	PRE_ControlesEntrenamiento(CTRL_ESPERA_BOTON_INICIO_PRUEBA);
	ENT_ConfirmaInicioRecorridoPrueba(&ENT_IniciarRecorridoPrueba, &ENT_DetenerPruebaBotonera);
	ENT_ModoPasoAPaso(&ENT_FuncionPasoAPaso, &ENT_ModoMotorABB);
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			EstablecePosicionActual
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Toma la posici�n actual de los enconder de los motores
.						y la establece como posici�n (0,0).
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int EstablecePosicionActual()
{
	static int i=0;
	
	if (i==0)
	{
		MOT_PosicionMotores(MOT_POSICION_ACTUAL);
		ENT_TorqueMotores(5.24, 2.5);
		MOT_EstadoMotores(MOT_HABILITAR);
	}
	else
	{
		MOT_EstadoMotores(MOT_DESHABILITAR);
	}
	
	i=!i;
	
	
	MOT_PosicionActual(&posRotActual, &posLinActual);
	minimoRotacional=posRotActual;
	minimoLineal=posLinActual;
	maximoRotacional=posRotActual;
	maximoLineal=posLinActual;
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			SetPosicionActual
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura el evento de la pantalla donde el usuario a
.						solicitado que se defina la posici�n actual como
.						el origen
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK SetPosicionActual (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			EstablecePosicionActual();
			break;
	}
	return 0;
}



/*****************************************************************************
.
. Funci�n C:			ImprimePosicion
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Manda a la pantalla la informaci�n de la posicion
.						actual de los enconders de los motores
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int ImprimePosicion()
{
	static int contador=1;
	
	printf("%d  -  L[%d,%d]  R[%d,%d] Diferencia en cuentas: L[%d]  R[%d]\n",contador++, minimoLineal, maximoLineal, minimoRotacional, maximoRotacional, maximoLineal-minimoLineal,
		maximoRotacional - minimoRotacional);
	
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			ActivaTimer
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Captura el evento de la pantalla para que se habilite
.						el timer que aydar� en el proceso de ubicaci�n del
.						sistema e informar que la palanca se encuentra 
.						cerca de....
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK ActivaTimer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (iPanelEntrenamiento, pEntrenar_tmrEliminar,
							  ATTR_ENABLED, 1);
			//ENT_AceptarPosicionTest01(&ImprimePosicion, &EstablecePosicionActual);
			//CanvasClear (iPanelEntrenamiento, pEntrenar_cnvDemo, VAL_ENTIRE_OBJECT);
			break;
	}
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			PosicionActual
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Timer que actualiza la posicion actual del sistema
.						y calcula que se encuentra cerca de...
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int CVICALLBACK PosicionActual (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			/*
			MOT_PosicionActual(&posRotActual, &posLinActual);
			
			if (posRotActual < minimoRotacional)
			{
				minimoRotacional = posRotActual;
			}
			
			if (posRotActual > maximoRotacional)
			{
				maximoRotacional = posRotActual;
			}
			
			if (posLinActual < minimoLineal)
			{
				minimoLineal = posLinActual;
			}
			
			if (posLinActual > maximoLineal)
			{
				maximoLineal = posLinActual;
			}
			
			SetCtrlVal (iPanelEntrenamiento, pEntrenar_numLinealMaximo, maximoLineal);
			SetCtrlVal (iPanelEntrenamiento, pEntrenar_numLinealMinimo, minimoLineal);
			SetCtrlVal (iPanelEntrenamiento, pEntrenar_numRotacionalMaximo, maximoRotacional);
			SetCtrlVal (iPanelEntrenamiento, pEntrenar_numRotacionalMinimo, minimoRotacional);
			
			int iAlto=0;
			int iAncho=0;
			
			int x = posRotActual;
			int y = posLinActual;
			int iColor=0;
			
			GetCtrlVal(iPanelEntrenamiento, pEntrenar_binColor, &iColor);
			
			
			GetCtrlAttribute(iPanelEntrenamiento, pEntrenar_cnvDemo, ATTR_HEIGHT, &iAlto);
			GetCtrlAttribute(iPanelEntrenamiento, pEntrenar_cnvDemo, ATTR_WIDTH, &iAncho);
			
			double dY = ((double)iAlto/(double)ALTO_LINEAL)*posLinActual*0.45;
			double dX = ((double)iAncho/(double)ANCHO_ROTACIONAL)*posRotActual*5.0;
			
			x = (iAncho/2) + (int)dX;
			y = (iAlto/2) + (int)dY;
			
			if (iColor == 0)
				SetCtrlAttribute (iPanelEntrenamiento, pEntrenar_cnvDemo, ATTR_PEN_FILL_COLOR,VAL_RED);
			else
				SetCtrlAttribute (iPanelEntrenamiento, pEntrenar_cnvDemo, ATTR_PEN_FILL_COLOR,VAL_GREEN);
			
			CanvasDrawOval (iPanelEntrenamiento, pEntrenar_cnvDemo, MakeRect(y,x,10,10),
							VAL_DRAW_INTERIOR);
			//CanvasDrawPoint (iPanelEntrenamiento, pEntrenar_cnvDemo, MakePoint(x,y));
			*/
			
			long iRotacional;
			long iLineal;
	
			MOT_PosicionActual(&iRotacional, &iLineal);
			SetCtrlVal (iPanelEntrenamiento, pEntrenar_strCercaDe, PAT_PosicionCercaDe(600, miLista, iRotacional, iLineal));
			
			break;
	}
	return 0;
}









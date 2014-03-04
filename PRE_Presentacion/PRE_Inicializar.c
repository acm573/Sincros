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
#include "mov_movimiento.h"
#include "com_comunicacion.h"
#include "pre_variables.h"
#include "bds_basededatos.h"
#include <formatio.h>


//prototipo de funciones
int MOT_CondicionesIniciales(void);
int MOT_CondicionSalida(void);
int MON_IniciaMonitoreo(void);
int MON_FinMonitoreo(void);
int MOT_EstadoMotores(stMotores Modo);
int DIG_CrearTarea(int ipanel, int icontrol);
int DIG_EstadoEntrada(int iEntrada);


/*****************************************************************************
.
. Funci�n C:			PRE_InicializarSistema
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Ejecuta la secuencia que establece las condiciones de
.						operaci�n iniciales del sistema de control
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_InicializarSistema()
{
	Fmt(MistPosicion.cDescripcion,"%s<%s","");
	MistPosicion.iLineal=0;
	MistPosicion.iNumeroVelocidad=0;
	MistPosicion.iPlano=0;
	MistPosicion.iRotacional=0;
	MistPosicion.iTipo=0;
	MistPosicion.x=0;
	MistPosicion.y=0;
	
	iConexion = -1;
	//miLista = NULL;
	
	//Inicia la aplicaci�n de comunicaci�n TCP
	COM_IniciaAplicacion();
	
	Delay(2.0);

	COM_IniciaClienteTCP();
	
	Delay(1.0);
	
	COM_EstructuraMensaje(1,10220,1,0);

	COM_EstructuraMensaje(1,10462,1,0);

	COM_EstructuraMensaje(1,10460,1,0);

	COM_EstructuraMensaje(1,10220,2,0);

	COM_EstructuraMensaje(1,10462,2,0);

	COM_EstructuraMensaje(1,10460,2,0);

	MOT_CondicionesIniciales();
	
	MON_IniciaMonitoreo();
	
	
	MOT_EstadoMotores(MOT_DESHABILITAR);
	DIG_CrearTarea(0, 0);
	
	//Registra las condiciones de operaci�n actual del sistema
	SetCtrlVal(iPanelPrincipal, pPrincipal_ledEquipoPXI, mov_VerificarPXI(Tarjeta_01));
	SetCtrlVal(iPanelPrincipal, pPrincipal_ledBotonRearme, DIG_EstadoEntrada(DIG_REARME));
	SetCtrlVal(iPanelPrincipal, pPrincipal_ledParoEmergencia, DIG_EstadoEntrada(DIG_PARO_EMERGENCIA));
	
	//inicializa la conexi�n con la base de datos
	BDS_Conexion();
	
	
	
	return 0;
}


/*****************************************************************************
.
. Funci�n C:			PRE_Finalizar
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Ejecuta la secuencia que establece las condiciones de
.						operaci�n iniciales del sistema de control
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int PRE_Finalizar()
{
	MON_FinMonitoreo();
	COM_FinalizaAplicacion();
	MOT_CondicionSalida();
	
	BDS_Cerrar();
	return 0;
}

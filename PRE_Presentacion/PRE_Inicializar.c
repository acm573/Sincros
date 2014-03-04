/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Archivo:					PRE_MensajeControles.c
. Propósito:				Administra los mensajes de ayuda que se despliegan
.							en la pantalla 
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		18 de Febrero de 2013
. Responsable modificación: 
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
. Función C:			PRE_InicializarSistema
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Ejecuta la secuencia que establece las condiciones de
.						operación iniciales del sistema de control
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
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
	
	//Inicia la aplicación de comunicación TCP
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
	
	//Registra las condiciones de operación actual del sistema
	SetCtrlVal(iPanelPrincipal, pPrincipal_ledEquipoPXI, mov_VerificarPXI(Tarjeta_01));
	SetCtrlVal(iPanelPrincipal, pPrincipal_ledBotonRearme, DIG_EstadoEntrada(DIG_REARME));
	SetCtrlVal(iPanelPrincipal, pPrincipal_ledParoEmergencia, DIG_EstadoEntrada(DIG_PARO_EMERGENCIA));
	
	//inicializa la conexión con la base de datos
	BDS_Conexion();
	
	
	
	return 0;
}


/*****************************************************************************
.
. Función C:			PRE_Finalizar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Ejecuta la secuencia que establece las condiciones de
.						operación iniciales del sistema de control
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	18 de Febrero de 2011
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

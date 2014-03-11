/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Archivo:					BDS_Lectura.c
. Propósito:				Contiene las funciones referentes al proceso de 
.							consulta de información a la base de datos.
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		11 de Marzo de 2014
. Responsable modificación: 
.
*******************************************************************************/
#include <userint.h>
#include "bds_basededatos.h"
#include <formatio.h>
#include "pre_tipos.h"

char *GRA_Strcat(int iNoElementos, ...);


/*****************************************************************************
.
. Función C:			BDS_LeerTransmisiones
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Obtiene la información de la transmisión que se ha
.						seleccionado en la pantalla.
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
stTransmisiones BDS_LeerTransmisiones(int iPanel, int iControl)
{
	stTransmisiones Modo = TRA_SIN_TRANSMISIONES;
	int iVista;				//referencia a la vista solicitada
	int iIdTransmision=0;			//campo
	char cIdentificador[250]={0};	//campo
	int iLado=0;					//campo
	int iNoVelocidades=0;			//campo
	
	//limpia la lista
	DeleteListItem(iPanel, iControl, 0, -1);
	
	//insertar aqui llamada a lectura de base de datos
	DBText("SELECT id_transmisiones, identificador, lado, no_velocidades FROM transmisiones",&iVista);
	DBMap(iVista, 4, 
		  	Int, &iIdTransmision, 
			String, cIdentificador, 
			Int, &iLado, 
			Int, &iNoVelocidades);
	
	while (DBFetchNext(iVista)==DB_SUCCESS)
	{
		InsertListItem(iPanel, iControl, -1, cIdentificador, iIdTransmision);
		Modo = TRA_CON_TRANSMISIONES;
	}
	
	if (Modo == TRA_CON_TRANSMISIONES)
		SetCtrlIndex(iPanel, iControl, 0);
	
	DBClear(iVista);
	return Modo;
}



/*****************************************************************************
.
. Función C:			BDS_LeerDetalleTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Obtiene la información de la transmisión que se ha
.						seleccionado en la pantalla y carga la información
.						en detalle.
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
stTransmisiones BDS_LeerDetalleTransmision(int iPanel, ...)
{
	stTransmisiones Modo = TRA_CORRECTO;
	va_list pa;
	int iNombreTransmision;
	int iNumeroVelocidades;
	int iTablaRelaciones;
	int iId;
	char cDescripcion[250]={0};
	int iNumVelocidades=0;
	char cId[5]={0};
	int iVista=0;
	int iNum_Velocidad;
	double dRelacion;
	
	va_start(pa, iPanel);
	
	//obtiene las referencias de los controles
	iNombreTransmision = va_arg(pa, int);
	iNumeroVelocidades = va_arg(pa, int);
	iTablaRelaciones = va_arg(pa, int);
	iId = va_arg(pa, int);
	
	Fmt(cId,"%s<%d",iId);
	//insertar aqui llamada a lectura de base de datos
	DBText(GRA_Strcat(2,
				   "SELECT identificador, no_velocidades FROM transmisiones WHERE id_transmisiones = ",
				   cId),&iVista);
	DBMap(iVista, 2, 
		  	String, cDescripcion, 
			Int, &iNumVelocidades);
	
	while (DBFetchNext(iVista)==DB_SUCCESS)
	{
		SetCtrlVal(iPanel, iNombreTransmision, cDescripcion);
		SetCtrlVal(iPanel, iNumeroVelocidades, iNumVelocidades);
	}
	
	DBClear(iVista);
	
	//ahora se obtiene la informacion de las relaciones de las velocidades
	//asociadas a la transmision que se ha seleccionado
	DBText(GRA_Strcat(3,
				   "SELECT numero_velocidad, relacion FROM relaciones WHERE fk_id_transmision = ",
				   cId,
					 " ORDER BY numero_velocidad DESC"),&iVista);
	DBMap(iVista, 2, 
		  	Int, &iNum_Velocidad, 
			Double, &dRelacion);
	
	DeleteTableRows (iPanel, iTablaRelaciones, 1, -1);
	
	while (DBFetchNext(iVista)==DB_SUCCESS)
	{
		//inserta un nuevo renglon
		InsertTableRows (iPanel, iTablaRelaciones, 1, 1,
						 VAL_USE_MASTER_CELL_TYPE);
		
		SetTableCellVal (iPanel, iTablaRelaciones, MakePoint(1,1), dRelacion);
	}
	
	DBClear(iVista);
	
	return Modo;
}

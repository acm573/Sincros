/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Archivo:					BDS_Eliminar.c
. Prop�sito:				Contiene las funciones para eliminar registros
.							de las diferentes tablas de la base de datos
.							del sistema
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		13 de Marzo de 2014
. Responsable modificaci�n: 
.
*******************************************************************************/
#include <userint.h>
#include "bds_basededatos.h"
#include <formatio.h>
#include "pre_tipos.h"

char *GRA_Strcat(int iNoElementos, ...);


/*****************************************************************************
.
. Funci�n C:			BDS_EliminarTransmision
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Procesa la solicitud de eliminar la informaci�n
.						referente a la transmision que se ha seleccionado
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	13 de Marzo de 2014
.
*****************************************************************************/
stTransmisiones BDS_EliminarTransmision(int iId)
{
	stTransmisiones Modo = TRA_CORRECTO;
	int iIdTransmision=0;			//campo
	char cIdentificador[250]={0};	//campo
	int iLado=0;					//campo
	int iNoVelocidades=0;			//campo
	char cQuery[1000]={0};			//comando SQL
	stQuery Query;					//estructura
	
	
	//elimina la transmision seleccionada y el sistema por reglas de la base
	//de datos, eliminar� toda la informaci�n relacionada
	Fmt(Query.cTexto, "%s<%s%d", 
	   	"DELETE FROM transmisiones WHERE id_transmisiones = ",
		iId);
	
	DBImmediateSQL(DBConnection(), Query.cTexto);
	return Modo;
}


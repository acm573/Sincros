/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Archivo:					BDS_Escritura.c
. Prop�sito:				Contiene las funciones referentes al proceso de 
.							escritura de informaci�n a la base de datos.
. Lenguaje:					LabWindows/CVI 13.0.0 (647)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		11 de Marzo de 2014
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
. Funci�n C:			BDS_InsertaTransmision
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Inserta en el sistema la nueva transmision
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	11 de Marzo de 2014
.
*****************************************************************************/
stTransmisiones BDS_InsertaTransmision(char *pcDescripcion, int iNumVelocidades,
	double dListaRelaciones[])
{
	stTransmisiones Modo = TRA_SIN_TRANSMISIONES;
	int iVista;				//referencia a la vista solicitada
	int iIdTransmision=0;			//campo
	char cIdentificador[250]={0};	//campo
	int iLado=0;					//campo
	int iNoVelocidades=0;			//campo
	
	
	//insertar aqui llamada a lectura de base de datos
	DBText("SELECT id_transmisiones, identificador, lado, no_velocidades FROM transmisiones",&iVista);
	DBMap(iVista, 4, 
		  	Int, &iIdTransmision, 
			String, cIdentificador, 
			Int, &iLado, 
			Int, &iNoVelocidades);
	
	while (DBFetchNext(iVista)==DB_SUCCESS)
	{
		
	}
	
	
	DBClear(iVista);
	return Modo;
}


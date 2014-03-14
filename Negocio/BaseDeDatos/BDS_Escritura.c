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
. Descripcion: 			Inserta la informacion de la nueva transmision que
.						se ha capturado en el sistema.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	11 de Marzo de 2014
.
*****************************************************************************/
stTransmisiones BDS_InsertaTransmision(char *pcDescripcion, int iNumVelocidades,
	double dListaRelaciones[], int iLadoManipulador)
{
	stTransmisiones Modo = TRA_SIN_TRANSMISIONES;
	int iIdTransmision=0;			//campo
	char cIdentificador[250]={0};	//campo
	int iLado=0;					//campo
	int iNoVelocidades=0;			//campo
	char cQuery[1000]={0};			//comando SQL
	stQuery Query;					//estructura
	
	
	//insertar aqui llamada a lectura de base de datos
	Fmt(Query.cTexto, "%s<%s%s%s%d%s%d%s", 
	   	"INSERT INTO transmisiones (id_transmisiones, identificador, lado, no_velocidades) VALUES (0, \"",
		pcDescripcion,
		"\", ",
		(iLadoManipulador==1)?0:1,
		",",
		iNumVelocidades,
	   ")");
	
	DBImmediateSQL(DBConnection(), Query.cTexto);

	//Realiza la consulta de la transmision que se ha insertado, para conocer su Id
	Fmt(Query.cTexto, "%s<%s%s%s", 
	   	"SELECT id_transmisiones FROM transmisiones WHERE identificador = \"",
		pcDescripcion,
		"\"");
	DBText(Query.cTexto,&Query.iVista);
	DBMap(Query.iVista, 1, Int, &iIdTransmision);
	DBFetchNext(Query.iVista);
	DBClear(Query.iVista);
	
	//inserta las relaciones de velocidad para el id de la transmision que se ha creado
	for (int i=0;i<iNumVelocidades;i++)
	{
		Fmt(Query.cTexto,"%s<%s%d%s%d%s%f%s",
				"INSERT INTO relaciones VALUES (0,",
		   		iIdTransmision,
				",",
				i+1,
				",",
				dListaRelaciones[i],
				")");
		DBImmediateSQL(DBConnection(), Query.cTexto);
	}
	return Modo;
}



/*****************************************************************************
.
. Funci�n C:			BDS_ActualizaTransmision
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Actualiza la informaci�n de la transmision que se ha
.						editado por parte del usuario.
. Par�metro de entrada:	ninguno
. Par�metro de salida:	cero
. Fecha de creaci�n:	13 de Marzo de 2014
.
*****************************************************************************/
stTransmisiones  BDS_ActualizaTransmision(int iId, int iNumVelocidades, 
		double dListaRelaciones[], int iLadoManipulador)
{
	stTransmisiones Modo = TRA_CORRECTO;
	char cIdentificador[250]={0};	//campo
	int iLado=0;					//campo
	int iNoVelocidades=0;			//campo
	char cQuery[1000]={0};			//comando SQL
	stQuery Query;					//estructura
	
	//insertar aqui llamada a proceso de actualizacio�n en la base de datos
	Fmt(Query.cTexto, "%s<%s%d%s%d%s%d", 
	   	"UPDATE transmisiones SET lado = ",
		(iLadoManipulador==1)?0:1,
		", no_velocidades = ",
		iNumVelocidades,
	   " WHERE id_transmisiones = ",
	   iId);
	DBImmediateSQL(DBConnection(), Query.cTexto);
	
	//ahora procese a actualizar la informaci�n de las relaciones de velocidad,
	//para ello, primero se eliminan las existentes y se insertan las nuevas
	
	Fmt(Query.cTexto, "%s<%s%d", 
	   	"DELETE FROM relaciones WHERE fk_id_transmision = ",
	    iId);
	DBImmediateSQL(DBConnection(), Query.cTexto);
	
	//finalmente vuelve a insertar la lista de relaciones que se han
	//configurado en el sistema
	for (int i=0;i<iNumVelocidades;i++)
	{
		Fmt(Query.cTexto,"%s<%s%d%s%d%s%f%s",
				"INSERT INTO relaciones VALUES (0,",
		   		iId,
				",",
				i+1,
				",",
				dListaRelaciones[i],
				")");
		DBImmediateSQL(DBConnection(), Query.cTexto);
	}
	return Modo;
}

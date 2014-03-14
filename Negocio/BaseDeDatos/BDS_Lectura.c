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
char *GRA_IntStr(int iValor);


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
	int iIdTransmision=0;			//campo
	char cIdentificador[250]={0};	//campo
	int iLado=0;					//campo
	int iNoVelocidades=0;			//campo
	stQuery Query;
	
	//limpia la lista
	DeleteListItem(iPanel, iControl, 0, -1);
	
	//insertar aqui llamada a lectura de base de datos
	DBText("SELECT id_transmisiones, identificador, lado, no_velocidades FROM transmisiones ORDER BY identificador",&Query.iVista);
	DBMap(Query.iVista, 4, 
		  	Int, &iIdTransmision, 
			String, cIdentificador, 
			Int, &iLado, 
			Int, &iNoVelocidades);
	
	while (DBFetchNext(Query.iVista)==DB_SUCCESS)
	{
		InsertListItem(iPanel, iControl, -1, cIdentificador, iIdTransmision);
		Modo = TRA_CON_TRANSMISIONES;
	}
	
	if (Modo == TRA_CON_TRANSMISIONES)
		SetCtrlIndex(iPanel, iControl, 0);
	
	DBClear(Query.iVista);
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
	int iIzquierdo;
	int iDerecho;
	int iId;
	int iLadoManipulador;
	char cDescripcion[250]={0};
	int iNumVelocidades=0;
	char cId[5]={0};
	stQuery Query;
	int iNum_Velocidad;
	double dRelacion;
	
	va_start(pa, iPanel);
	
	//obtiene las referencias de los controles
	iNombreTransmision = va_arg(pa, int);
	iNumeroVelocidades = va_arg(pa, int);
	iTablaRelaciones = va_arg(pa, int);
	iIzquierdo = va_arg(pa, int);
	iDerecho = va_arg(pa, int);
	iId = va_arg(pa, int);
	
	Fmt(cId,"%s<%d",iId);
	//insertar aqui llamada a lectura de base de datos
	DBText(GRA_Strcat(2,
				   "SELECT identificador, no_velocidades, lado FROM transmisiones WHERE id_transmisiones = ",
				   cId),&Query.iVista);
	DBMap(Query.iVista, 3, 
		  	String, cDescripcion, 
			Int, &iNumVelocidades,
		 	Int, &iLadoManipulador);
	
	while (DBFetchNext(Query.iVista)==DB_SUCCESS)
	{
		SetCtrlVal(iPanel, iNombreTransmision, cDescripcion);
		SetCtrlVal(iPanel, iNumeroVelocidades, iNumVelocidades);
		SetCtrlVal(iPanel, iIzquierdo, !iLadoManipulador);
		SetCtrlVal(iPanel, iDerecho, iLadoManipulador);
	}
	DBClear(Query.iVista);
	
	//ahora se obtiene la informacion de las relaciones de las velocidades
	//asociadas a la transmision que se ha seleccionado
	DBText(GRA_Strcat(3,
				   "SELECT numero_velocidad, relacion FROM relaciones WHERE fk_id_transmision = ",
				   cId,
					 " ORDER BY numero_velocidad DESC"),&Query.iVista);
	DBMap(Query.iVista, 2, 
		  	Int, &iNum_Velocidad, 
			Double, &dRelacion);
	
	DeleteTableRows (iPanel, iTablaRelaciones, 1, -1);
	while (DBFetchNext(Query.iVista)==DB_SUCCESS)
	{
		//inserta un nuevo renglon
		InsertTableRows (iPanel, iTablaRelaciones, 1, 1,
						 VAL_USE_MASTER_CELL_TYPE);
		SetTableCellVal (iPanel, iTablaRelaciones, MakePoint(1,1), dRelacion);
	}
	DBClear(Query.iVista);
	
	return Modo;
}




/*****************************************************************************
.
. Función C:			BDS_VerificaDescripcionTransmision
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Verifica en la base de datos si ya existe alguna
.						transmision que este utilizando el mismo nombre
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
stTransmisiones BDS_VerificaDescripcionTransmision(char  *pcNombre)
{
	stTransmisiones Modo = TRA_NO_EXISTE;
	stQuery Query;
	
	DBText(GRA_Strcat(3,
				   "SELECT * FROM transmisiones WHERE identificador = \"",
				   pcNombre,"\""),&Query.iVista);
	
	while (DBFetchNext(Query.iVista)==DB_SUCCESS)
	{
		Modo = TRA_EXISTE;
	}
	
	DBClear(Query.iVista);
	
	return Modo;
}



/*****************************************************************************
.
. Función C:			BDS_InformeParaEliminar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Responde con un mensaje, indicando un reporte de la
.						información vinculada a la transmisión.
. Parámetro de entrada:	ninguno
. Parámetro de salida:	cero
. Fecha de creación:	11 de Marzo de 2014
.
*****************************************************************************/
char *BDS_InformeParaEliminar(int iId, char *pcDescripcion, int iNumVelocidades)
{
	static char cMensaje[1000]={0};
	int iRegistros=0;
	stQuery Query;
	
	FillBytes(cMensaje,0,1000,0);
	
	strcat(cMensaje, "Se ha seleccionado para eliminar la transmisión denominada \"");
	strcat(cMensaje,  pcDescripcion);
	strcat(cMensaje,  "\" que cuenta con ");
	strcat(cMensaje,  GRA_IntStr(iNumVelocidades));
	strcat(cMensaje,  " velocidades configuradas.\n\n");
	
	//verifica si tiene una secuencias que dependan de ella
	DBText(GRA_Strcat(2,
				   "SELECT * FROM secuencias WHERE fk_id_transmisiones = ",
				   GRA_IntStr(iId)), &Query.iVista);
	
	while (DBFetchNext(Query.iVista)==DB_SUCCESS)
	{
		iRegistros++;
	}
	DBClear(Query.iVista);	
	
	if (iRegistros>0)
	{
		strcat(cMensaje,  "Se ha identifcado que existe(n) ");
		strcat(cMensaje,  GRA_IntStr(iRegistros));
		strcat(cMensaje,  " configuracion(es) asociadas con esta transmisión. El sistema eliminará automáticamente del\n");
		strcat(cMensaje,  "catálogo de secuencias este(os) elemento(s), una vez que se confirme este mensaje. \n\n");
		strcat(cMensaje,  "Al eliminar una secuencia, el sistema incluirá en el proceso de borrado, la información de");
		strcat(cMensaje,  " aquellas pruebas que no hayan\nconcluido en este momento.\n\n\n");
	}
	else
	{
		strcat(cMensaje,  "No se identificaron dependencias con alguna configuración de secuencia en el sistema.\n\n\n");
	}
	
	strcat(cMensaje,  "                                                              ");
	strcat(cMensaje, "¿Desea continuar con el proceso de eliminar?\n");
	
	return cMensaje;
}

//==============================================================================
//
// Title:		BDS_Funciones.c
// Purpose:		A short description of the implementation.
//
// Created on:	04/03/2014 at 01:38:18 p.m. by CIDESI.
// Copyright:	CIDESI. All Rights Reserved.
//
//==============================================================================



#include "BDS_BaseDeDatos.h"
#include <userint.h>
#include <utility.h>
#include <formatio.h>


//constantes para el manejo de las funciones de base de datos
int iConexion=-1;


int DBConnection()
{
	int iError=0;
	
	//carga en cero el manenador de la conexion a base de datos
	if (iConexion < 0)
	{
		SetBreakOnLibraryErrors (0);
		iConexion = DBConnect("sincros");
		SetBreakOnLibraryErrors (1);
	
		if (iConexion <= 0)
		{
			iError = DBError ();
			MessagePopup ("Error al conectarse a la base de datos", DBErrorMessage ());
			iConexion=-1;
		}
	}
	return iConexion;
}


int DBClose()
{
	if (iConexion > 0)
	{
		SetBreakOnLibraryErrors (0);
		DBCloseConnection(iConexion);
		DBDiscardConnection(iConexion);
		SetBreakOnLibraryErrors (0);
	}
	return 0;
}


int DBText(char *pcCadena, int *piVista)
{
	*piVista = DBActivateSQL (DBConnection(), pcCadena);
	return 0;
}


int DBClear(iVista)
{
	DBDeactivateSQL(iVista);
	return 0;
}

int DBMap(int iVista, int iNoElementos, ...)
{
	va_list pa;
	stBDSTipo dato;
	int	i=0;
	int iError=0;
	int *piVarInt;
	double *pdVarDouble;
	char *pcVarString;
	
	va_start(pa, iNoElementos);
	
	for (i=0;i<iNoElementos;i++)
	{
		dato = va_arg(pa,int);
		
		switch (dato)
		{
			//Información para un tipo doble
			case Int:
				piVarInt = va_arg(pa,int*);
				DBBindColInt (iVista, i+1, piVarInt, &iError);
				break;
				
			case Double:
				pdVarDouble = va_arg(pa,double*);
				DBBindColDouble (iVista, i+1, pdVarDouble, &iError);
				break;
				
			//Información para un String
			case String:
				pcVarString = va_arg(pa, char*);
				DBBindColChar (iVista, i+1, 250, pcVarString, &iError, "");
				break;
		}
	}
	return 0;
}

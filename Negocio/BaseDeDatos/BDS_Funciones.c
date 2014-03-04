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


int BDS_Conexion()
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


int BDS_Cerrar()
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

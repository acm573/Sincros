/******************************************************************************
.
. Centro de Ingenier�a y Desarrollo Indusrial
. Nombre del Aplicativo:	M�dulo con funciones comunes diversas
. Archivo:					UTL_Utilerias.c
. Prop�sito:				Contiene diversas funciones para la ejecuci�n de
.							rutinas comunes en la aplicaci�n
. Lenguaje:					LabWindows/CVI 10.0.1 (434)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creaci�n:		18 de Febrero de 2013
. Responsable modificaci�n: 
.
*******************************************************************************/

//Includes
#include "toolbox.h"
#include "regexpr.h"
#include <ansi_c.h>
#include <formatio.h>



/*****************************************************************************
.
. Funci�n C:			UTL_Split 
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Separa una cadena en N partes utilizando un patron de
.						texto como separador
. Par�metro de entrada:	char *pcCadena | contiene la cadena a analizar
.						char *pcPatron | cadena que se utilizar� como separador
.						char *c[] | arreglo donde se colocar�n los elementos
.									que se han separado
.						int *total | referencia en donde se almacenar� el total
.									 del n�mero de elementos que fueron 
									 encontrados en la cadena original
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
void * UTL_Split (char *pcCadena, char *pcPatron, char (*c)[30], int *total)
{
    int iResultado;
	int iLongitud;
	int iPosicion;
	int iEncontrado;
	char cCadena[260]={0};
	int gMatchPosition=0;
	ListType miLista;
	int iIndice=0;
	
	
	miLista = ListCreate (sizeof(cCadena));
	iResultado=0;
	while (iResultado==0)
	{
		iResultado = RegExpr_FindPatternInText (pcPatron, 0, pcCadena+gMatchPosition,
												(int)strlen(pcCadena+gMatchPosition),
												RegExpr_SearchForwards,
												RegExpr_MatchLargestNumChars,
												&iEncontrado, &iPosicion,
												&iLongitud);
		
		CopyString (cCadena, 0, pcCadena+gMatchPosition, 0, iPosicion);
		
		if (strlen(cCadena)!=0)
		{
			gMatchPosition += iPosicion+iLongitud;
			Fmt(c[iIndice],"%s<%s",cCadena);
			iIndice++;
			*total = iIndice;
		}
		else
			iResultado=1;
	}
	
	return 0;
}




/*****************************************************************************
.
. Funci�n C:			UTIL_Wait 
. Responsable:			C�sar Armando Cruz Mendoza
. Descripcion: 			Implementa una pausa
. Par�metro de entrada:	double delay | valor double de retardo en segundos
. Par�metro de salida:	cero
. Fecha de creaci�n:	18 de Febrero de 2011
.
*****************************************************************************/
int UTIL_Wait (double delay) 
{
	static double time1;
	static double time2;
	
	time1 = Timer();
	time2 = Timer();
	
	while ((time2-time1) < delay)
	{
		time2 = Timer();
		ProcessSystemEvents();
	}
	
	return 0;
}

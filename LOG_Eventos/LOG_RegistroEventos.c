/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Nombre del Aplicativo:	Monitor de eventos en sistema
. Archivo:					LOG_RegistroEventos.c
. Propósito:				Contiene las funciones para el registro de eventos
.							al interior de la aplicación
. Lenguaje:					LabWindows/CVI 9.1.1 (450)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		17 de Mayo de 2011
. Responsable modificación: 
.
*******************************************************************************/

//Includes
#include "var_registroeventos.h"
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>


//variables globales
static char cNombreArchivo[260]={0}; //nombre del archivo
static int iEstado=0;	//0 No inicializado  1 Inicializado
static STTipo Modalidad=LOG_DESHABILITAR;	//por defecto se encuentra deshabilitado


/*****************************************************************************
.
. Función C:			LOG_Inicializar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Funcion que inicializa el proceso de registro
. Parámetro de entrada:	ninguno
. Parámetro de salida:	ninguno
. Fecha de creación:	17 de Mayo de 2011
.
*****************************************************************************/
void LOG_Inicializar()
{
	int iArchivo=0;	//referencia al nombre de archivo
	char cBuffer[500]={0}; //cadena de texto a escribir en archivo
	
	Fmt(cNombreArchivo,"%s<%s%s%s%s%s","LOG_",DateStr(),"_",TimeStr(),".txt");
	
	//Crea el archivo
	cNombreArchivo[strlen(cNombreArchivo)-7]='_';
	cNombreArchivo[strlen(cNombreArchivo)-10]='_';
	iArchivo = OpenFile (cNombreArchivo, VAL_WRITE_ONLY, VAL_TRUNCATE,
						 VAL_ASCII);
	
	Fmt(cBuffer,"%s<%s%s%s%s%s%s%s%s%s%s","Nombre de archivo: ",cNombreArchivo,"\n",
		"Fecha: ",DateStr(),"\n",
		"Hora: ",TimeStr(),"\n",
		"--------------------------------------------------------------------");

	WriteFile(iArchivo,cBuffer,sizeof(cBuffer));
	
	CloseFile(iArchivo);
	
	iEstado = 1;
}




/*****************************************************************************
.
. Función C:			LOG_Insertar
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Funcion que se encarga de insertar en el archivo
.						log la información indicada
. Parámetro de entrada:	int iTipo | Define el tipo de información a recibir
. Parámetro de salida:	ninguno
. Fecha de creación:	18 de Mayo de 2011
.
*****************************************************************************/
void LOG_Insertar(STTipo Tipo, ...)
{
	int iArchivo=0;	//referencia al nombre de archivo
	char cBuffer[500]={0}; //cadena de texto a escribir en archivo
	char *pcFuncion=NULL; //apuntador a la cadena con el nombre de funcion
	char *pcArchivo=NULL; //apuntador a la cadena con el nombre del archivo
	char *pcMensaje=NULL; //apuntador a la cadena personalizada
	int iEntero=0; //variable para valor entero
	double dDoble=0; //variable para el valor doble
	va_list pe; //Inicializa la lista de parametros
	
	//Si no fue inicializado se cancela	
	if (iEstado == 0)
		return;
	
	va_start(pe, Tipo);
	
	iArchivo = OpenFile (cNombreArchivo, VAL_WRITE_ONLY, VAL_APPEND,
						 VAL_ASCII);

	switch (Tipo)
	{
		case LOG_INSERTAR:
			pcFuncion = va_arg(pe,char*);
			pcArchivo = va_arg(pe,char*);
			Fmt (cBuffer, "%s<%s%s%s%s%s%s",
				 "\n", TimeStr(), "\tArchivo: ", pcArchivo, "\t\tFunción: ", pcFuncion);
			break;
	
		case LOG_CADENA:
			pcFuncion = va_arg(pe,char*);
			pcArchivo = va_arg(pe,char*);
			pcMensaje = va_arg(pe,char*);
			Fmt (cBuffer, "%s<%s%s%s%s%s%s%s%s",
				 "\n", TimeStr(), "\tArchivo: ", pcArchivo, "\t\tFunción: ", pcFuncion,"\t\t",pcMensaje);
		
			break;
			
		case LOG_ENTERO:
			pcFuncion = va_arg(pe,char*);
			pcArchivo = va_arg(pe,char*);
			iEntero = va_arg(pe,int);
			pcMensaje = va_arg(pe,char*);
			
			Fmt (cBuffer, "%s<%s%s%s%s%s%s%s%s%d%s%s%s",
				 "\n", 
				 TimeStr(), 
				 "\tArchivo: ", 
				 pcArchivo, 
				 "\t\tFunción: ", 
				 pcFuncion,
				 "\t\t",
				 "[",
				 iEntero,
				 "]",
				 "\t\t",
				 pcMensaje);
			
			break;
			
		case LOG_DOBLE:
			pcFuncion = va_arg(pe,char*);
			pcArchivo = va_arg(pe,char*);
			dDoble = va_arg(pe,double);
			pcMensaje = va_arg(pe,char*);
			
			Fmt (cBuffer, "%s<%s%s%s%s%s%s%s%s%f%s%s%s",
				 "\n", 
				 TimeStr(), 
				 "\tArchivo: ", 
				 pcArchivo, 
				 "\t\tFunción: ", 
				 pcFuncion,
				 "\t\t",
				 "[",
				 dDoble,
				 "]",
				 "\t\t",
				 pcMensaje);
			
			break;
		
	}

	WriteFile(iArchivo,cBuffer,sizeof(cBuffer));

	CloseFile(iArchivo);
}




/*****************************************************************************
.
. Función C:			LOG_Monitor
. Responsable:			César Armando Cruz Mendoza
. Descripcion: 			Funcion que define las acciones del monitoreo
. Parámetro de entrada:	STMonitor Opcion | Tipo de accion
.						... | parametros variables
. Parámetro de salida:	ninguno
. Fecha de creación:	17 de Mayo de 2011
.
*****************************************************************************/
///HIFN Función que determina el comportamiento del monitoreo en el sistema. 
///HIFN La información queda almacenada en un archivo de texto con la
///HIFN siguiente nomenclatura: LOG_DDMMAA_HHMMSS.txt 
///HIFN Cuando se utiliza el parametro LOG_INSERTAR, se deberá acompañar con 
///HIFN 2 parámetros adicionales: __FILE__, __FUNCTION__
///HIFN Para el caso del LOG_PERSONALIZADO, se deberá acompañar con 
///HIFN 3 parámetros adicionales: __FILE__, __FUNCTION__, una cadena con la
///HIFN descripción personalizada deseada
///HIPAR Opcion/LOG_INSERTAR Inserta en el archivo la información de monitoreo
///HIPAR Opcion/LOG_CADENA Inserta un registro con mensaje 
///HIPAR Opcion/LOG_ENTERO Inserta un registro con valor entero y mensaje 
///HIPAR Opcion/LOG_HABILITA Habilita las funciones de monitoreo
///HIPAR Opcion/LOG_DESHABILITA Deshabilita las funciones de monitoreo
int LOG_Monitor(STTipo Opcion,...)
{
	va_list pa; //Inicializa la lista de parametros
	char *pcFuncion=NULL; //apuntador a la cadena con el nombre de funcion
	char *pcArchivo=NULL; //apuntador a la cadena con el nombre del archivo
	char *pcMensaje=NULL; //apuntador a la cadena personalizada
	int iEntero=0; //variable para el valor entero
	double dDoble=0; //variable para el valor doble
	
	va_start(pa, Opcion);
	
	switch (Opcion)
	{
		case LOG_HABILITAR:
			Modalidad = LOG_HABILITAR;
			break;
		
		case LOG_DESHABILITAR:
			Modalidad = LOG_DESHABILITAR;
			break;
			
		default:
			//si el monitoreo fue habilitado procesa las funciones siguientes
			if (Modalidad == LOG_HABILITAR)
			{
				switch (Opcion)
				{
					case LOG_INSERTAR:
						if (iEstado == 0)
							LOG_Inicializar();
						//requiere extraer los 2 parámetros adicionales de la funcion
						pcFuncion = va_arg(pa,char*);
						pcArchivo = va_arg(pa,char*);
			
						LOG_Insertar(Opcion,pcFuncion,pcArchivo);
						break;
			
					case LOG_CADENA:
						if (iEstado == 0)
							LOG_Inicializar();
						
						//requiere extraer los 3 parámetros adicionales de la funcion
						pcFuncion = va_arg(pa,char*);
						pcArchivo = va_arg(pa,char*);
						pcMensaje = va_arg(pa,char*);
			
						if (pcMensaje==NULL)
							LOG_Insertar(Opcion,pcFuncion,pcArchivo," ");
						else
							LOG_Insertar(Opcion,pcFuncion,pcArchivo,pcMensaje);
						
						break;
						
					case LOG_ENTERO:
						if (iEstado == 0)
							LOG_Inicializar();
						
						//requiere extraer los 3 parámetros adicionales de la funcion
						pcFuncion = va_arg(pa,char*);
						pcArchivo = va_arg(pa,char*);
						iEntero = va_arg(pa,int);
						pcMensaje = va_arg(pa,char*);
						
						if (pcMensaje==NULL)
							LOG_Insertar(Opcion,pcFuncion,pcArchivo,iEntero," ");
						else
							LOG_Insertar(Opcion,pcFuncion,pcArchivo,iEntero,pcMensaje);
						
						break;
						
					case LOG_DOBLE:
						if (iEstado == 0)
							LOG_Inicializar();
						
						//requiere extraer los 3 parámetros adicionales de la funcion
						pcFuncion = va_arg(pa,char*);
						pcArchivo = va_arg(pa,char*);
						dDoble = va_arg(pa,double);
						pcMensaje = va_arg(pa,char*);
			
						if (pcMensaje==NULL)
							LOG_Insertar(Opcion,pcFuncion,pcArchivo,dDoble," ");
						else
							LOG_Insertar(Opcion,pcFuncion,pcArchivo,dDoble,pcMensaje);
						
						break;
				}
			}
			break;
	}
	
	return 0;
}





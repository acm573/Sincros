/******************************************************************************
.
. Centro de Ingeniería y Desarrollo Indusrial
. Nombre del Aplicativo:	Variables de monitor de eventos en sistema
. Archivo:					VAR_RegistroEventos.h
. Propósito:				Contiene las variables para el registro de eventos
.							al interior de la aplicación
. Lenguaje:					LabWindows/CVI 9.1.1 (450)
. Autor:					Cesar Armando Cruz Mendoza
. Historia
. Fecha de Creación:		17 de Mayo de 2011
. Responsable modificación: 
.
*******************************************************************************/

//defines
#define Log_Habilitar (LOG_Monitor(LOG_HABILITAR));
#define Log_Deshabilitar (LOG_Monitor(LOG_DESHABILITAR));
#define Log_Registro (LOG_Monitor(LOG_INSERTAR, __FUNCTION__, __FILE__));
#define Log_Cadena(cadena) (LOG_Monitor(LOG_CADENA, __FUNCTION__, __FILE__,cadena));
#define Log_Entero(entero,cadena) (LOG_Monitor(LOG_ENTERO, __FUNCTION__, __FILE__,entero,cadena));
#define Log_Doble(doble,cadena) (LOG_Monitor(LOG_DOBLE, __FUNCTION__, __FILE__,doble,cadena));



//variables
typedef enum
{
	LOG_INSERTAR,
	LOG_CADENA,
	LOG_ENTERO,
	LOG_DOBLE,
	LOG_HABILITAR,
	LOG_DESHABILITAR
}STTipo;

	

//prototipos
int LOG_Monitor(STTipo Tipo,...);

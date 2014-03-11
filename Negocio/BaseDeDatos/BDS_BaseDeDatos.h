#include "cvi_db.h"


typedef enum
{
	Char,
	Short,
	Int,
	Float,
	Double,
	Binary,
	String,
	Date
}stBDSTipo;


//prototipo de funciones de acceso a base de datos
int DBConnection(void);
int DBClose(void);
int BDS_LeerTransmisiones(int iPanel, int iControl);
int DBText(char *pcCadena, int *piVista);
int DBMap(int iVista, int iNoElementos, ...);
int DBNext(int iVista);
int DBClear(int iVista);

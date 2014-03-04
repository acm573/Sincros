#include "cvi_db.h"


//constantes para el manejo de las funciones de base de datos
int iConexion;


//prototipo de funciones de acceso a base de datos
int BDS_Conexion(void);
int BDS_Cerrar(void);
int BDS_LeerTransmisiones(int iPanel, int iControl);

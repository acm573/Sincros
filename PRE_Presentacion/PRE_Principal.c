#include <cvirte.h>		
#include <userint.h>
#include "pre_variables.h"


//aqui van los comentarios
//nuevo comentario desde el sitio web
lkjkjljljljklj

int main (int argc, char *argv[])
{
	//carga las referencias de todos los paneles
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((iPanelPrincipal = LoadPanel (0, "PRE_Principal.uir", pPrincipal)) < 0)
		return -1;
	
	if ((iPanelMenuPrincipal = LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pMenu)) < 0)
		return -1;
	
	if ((iPanelCatTransmisiones = LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pCatTransm)) < 0)
		return -1;
	
	if ((iPanelEntrenamiento= LoadPanel (iPanelPrincipal, "PRE_Principal.uir", pEntrenar)) < 0)
		return -1;
	
	PRE_InicializarInterfaz();
	PRE_InicializarSistema();
	
	DisplayPanel (iPanelPrincipal);
	
	RunUserInterface ();
	DiscardPanel (iPanelPrincipal);
	return 0;
}



int CVICALLBACK PRE_Principal (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			PRE_FinalizarAplicacion();
			break;
			
		case EVENT_PANEL_SIZING:
			PRE_AjustarComponentes(eventData2);
			break;
		case EVENT_PANEL_SIZE:
			PRE_AjustarComponentes(eventData2);
			break;
			
		case EVENT_LEFT_CLICK:
			PRE_AnimacionMenu(OCULTAR);
			break;
	}
	return 0;
}



int CVICALLBACK PRE_MostrarMenu (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			PRE_ConfiguraMenu();
			break;
			
		case EVENT_MOUSE_POINTER_MOVE:
			PRE_BarraEstado("Despliega el men� principal del sistema.");
			break;
	}
	return 0;
}



int PRE_FinalizarAplicacion()
{
	if (ConfirmPopup ("Finalizar sesi�n con el sistema",
				  "�Desea cerrar la aplicaci�n?")==1)
	{
		PRE_Finalizar();
		QuitUserInterface (0);	
		
	}   
	return 0;
}





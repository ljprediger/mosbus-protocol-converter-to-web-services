/*
    FreeRTOS V7.3.0 - Copyright (C) 2012 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* Standard includes. */
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* lwIP core includes */
#include "lwip/opt.h"
#include "lwip/tcpip.h"

/* lwIP netif includes */
#include "netif/etharp.h"

/* applications includes */
#include "apps/httpserver_raw/httpd.h"

//Modificado
#include "main.h"
/*Include Modbus*/
#include "Lib_Modbus.h"
#include "uart.h"

/* The constants that define the IP address, net mask, gateway address and MAC
address are located at the bottom of FreeRTOSConfig.h. */
#define LWIP_PORT_INIT_IPADDR(addr)   IP4_ADDR((addr), configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3 )
#define LWIP_PORT_INIT_GW(addr)       IP4_ADDR((addr), configGW_IP_ADDR0, configGW_IP_ADDR1, configGW_IP_ADDR2, configGW_IP_ADDR3 )
#define LWIP_PORT_INIT_NETMASK(addr)  IP4_ADDR((addr), configNET_MASK0,configNET_MASK1,configNET_MASK2,configNET_MASK3)
#define LWIP_MAC_ADDR_BASE            { configMAC_ADDR0, configMAC_ADDR1, configMAC_ADDR2, configMAC_ADDR3, configMAC_ADDR4, configMAC_ADDR5 }

/* Definitions of the various SSI callback functions within the pccSSITags 
array.  If pccSSITags is updated, then these definitions must also be updated. */
#define ssiTASK_STATS_INDEX			0
#define ssiRUN_TIME_STATS_INDEX		1
#define ssiHMIBALANZA_INDEX			2


void HMI_Balanza(int8_t* pcBuffer){

	char aux[5];
	struct mb_pdu_decimal pregunta;
	struct mb_pdu_decimal respuesta;
	pregunta.datos.__ptr=(unsigned char*)pvPortMalloc(5*sizeof(unsigned char));
	respuesta.datos.__ptr=(unsigned char*)pvPortMalloc(20*sizeof(unsigned char));
	pregunta.ID_escalvo=2;
	pregunta.funcion=3;
	pregunta.datos.__size=4;
	pregunta.datos.__ptr[0]=0;
	pregunta.datos.__ptr[1]=11;
	pregunta.datos.__ptr[2]=0;
	pregunta.datos.__ptr[3]=4;
	unsigned int peso;

	/*Consulto a la balanza el estado y el peso*/


	//Envío consulta a la tarea ModbusMaster para procesar
	xQueueSend(xQueueModbusPDU, &pregunta, 0);
	//Depierto la tarea ModbusMaster
	xSemaphoreGive( xSemaphoreHandleSinc);
	//Espero a que la tarea ModbusMaster procece petición y la devuelvo al clinete web
	xQueueReceive(xQueueModbusPDU, &respuesta, portMAX_DELAY);

/*
	//Variables que indica si existe error
	mb_error error=No_Error;

	//Verfifico que petición enviada es correcta, si no es así no proceso petición
	error=mb_master_check_preg(pregunta);

	//Si no existe error proceso peticion
	if(error==No_Error)
		error=mb_master_procesa_peticion(pregunta,&respuesta);

	//Si existe error armo la respuesta dependiendo del error
	if(error!=No_Error)
		mb_master_exception_error(&pregunta,&respuesta,error);
*/

	if(respuesta.datos.__size==9){
		sprintf(( int8_t * )pcBuffer,"Estado Acual:       ");
		switch(respuesta.datos.__ptr[8]){
		case 1:
			strcat( pcBuffer,"LLenando recipiente" );
			break;
		case 4:
			strcat( pcBuffer, "Corte Grueso");
			break;
		case 7:
			strcat( pcBuffer, "Confirmacion de peso");
			break;
		case 10:
			strcat( pcBuffer, "Despacho recipiente" );
			break;
		case 13:
			strcat( pcBuffer, "Preparado para llenado");
			break;
		}
		strcat( pcBuffer, "\nPeso Acual:         ");
		//Calculo peso
		if(respuesta.datos.__ptr[1]<128){
			peso=256*respuesta.datos.__ptr[1]+respuesta.datos.__ptr[2];
		}else{
			peso=65536-(256*respuesta.datos.__ptr[1]+respuesta.datos.__ptr[2]);
		}
		sprintf(aux,"%u g",peso);
		strcat( pcBuffer, aux);
	}else{
		sprintf(aux,"Error respuesta del esclavo, verifique funcionamiento");
	}

	vPortFree(pregunta.datos.__ptr);
	vPortFree(respuesta.datos.__ptr);
}
/*-----------------------------------------------------------*/

/*
 * The function that implements the lwIP based sockets command interpreter
 * server.
 */
extern void vBasicSocketsCommandInterpreterTask( void *pvParameters );

/*
 * The SSI handler callback function passed to lwIP.
 */
static uint16_t uslwIPAppsSSIHandler( int iIndex, char *pcBuffer, int iBufferLength );

/*-----------------------------------------------------------*/

/* The SSI strings that are embedded in the served html files.  If this array
is changed, then the index position defined by the #defines such as 
ssiTASK_STATS_INDEX above must also be updated. */
static const char *pccSSITags[] = 
{
	"rtos_stats",
	"run_stats",
	"HMI_Balanza"
};

/*-----------------------------------------------------------*/

/* Called from the TCP/IP thread. */
void lwIPAppsInit( void *pvArgument )
{
ip_addr_t xIPAddr, xNetMask, xGateway;
extern err_t ethernetif_init( struct netif *xNetIf );
static struct netif xNetIf;

	( void ) pvArgument;

	/* Set up the network interface. */
	ip_addr_set_zero( &xGateway );
	ip_addr_set_zero( &xIPAddr );
	ip_addr_set_zero( &xNetMask );

	LWIP_PORT_INIT_GW(&xGateway);
	LWIP_PORT_INIT_IPADDR(&xIPAddr);
	LWIP_PORT_INIT_NETMASK(&xNetMask);

	netif_set_default( netif_add( &xNetIf, &xIPAddr, &xNetMask, &xGateway, NULL, ethernetif_init, tcpip_input ) );
	netif_set_up( &xNetIf );

	/* Install the server side include handler. */
	http_set_ssi_handler( uslwIPAppsSSIHandler, pccSSITags, sizeof( pccSSITags ) / sizeof( char * ) );

	/* Create the httpd server from the standard lwIP code.  This demonstrates
	use of the lwIP raw API. */
	httpd_init();

	/* Create the FreeRTOS defined basic command server.  This demonstrates use
	of the lwIP sockets API. */
	//xTaskCreate( vBasicSocketsCommandInterpreterTask, ( int8_t * ) "CmdInt", configCOMMAND_INTERPRETER_STACK_SIZE, NULL, configCOMMAND_INTERPRETER_TASK_PRIORITY, NULL );
}
/*-----------------------------------------------------------*/

static uint16_t uslwIPAppsSSIHandler( int iIndex, char *pcBuffer, int iBufferLength )
{
static unsigned int uiUpdateCount = 0;
static char cUpdateString[ 200 ];
extern char *pcMainGetTaskStatusMessage( void );



	// Unused parameter.
	( void ) iBufferLength;

	// The SSI handler function that generates text depending on the index of the SSI tag encountered
	
	switch( iIndex )
	{
		case ssiTASK_STATS_INDEX :
			vTaskList( ( int8_t * ) pcBuffer );
			break;

		case ssiRUN_TIME_STATS_INDEX :
			vTaskGetRunTimeStats( ( int8_t * ) pcBuffer );
			break;

		//AGREGADO
		case ssiHMIBALANZA_INDEX :
			HMI_Balanza(( int8_t * ) pcBuffer );
			break;
	}

	// Include a count of the number of times an SSI function has been executed in the returned string.
	uiUpdateCount++;
	sprintf( cUpdateString, "\r\n\r\nRefresh count %u", uiUpdateCount );
	strcat( pcBuffer, cUpdateString );
	return strlen( pcBuffer );
}


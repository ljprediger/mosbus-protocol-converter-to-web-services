/*
 * TASK_WEB_SERVICE.c
 *
 *  Created on: 26/05/2015
 *      Author: Leandro
 */

/*Include principal*/
#include "main.h"

/* lwIP includes. */
#include "lwip/tcpip.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

/*gsoap includes. */
#include "gsoap-lwip-io.h"
#include "modbus.nsmap"


/*Tarea encargada del Web Services*/
void pxTareaWebService ( void *pvParameters )
{

	  SOAP_SOCKET m, s; /* master and slave sockets */
	  struct soap soap, *tsoap;
	  soap_init(&soap);
	  //registro del plugin
	  gsoap_lwip_register_handle(&soap);
	   tsoap = soap_copy(&soap);
		m = soap_bind(tsoap, "192.168.0.200", 24, 100);
	    if (!soap_valid_socket(m))
	    { soap_print_fault(tsoap, stderr);
	      vTaskDelete( NULL );
	    }
	    fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
	    for ( ; ; )
	    { s = soap_accept(tsoap);
	      fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
	      if (!soap_valid_socket(s))
	      { soap_print_fault(tsoap, stderr);
	        vTaskDelete( NULL );
	      }
	      soap_serve(tsoap);
	      soap_end(tsoap);
	    }
		soap_done(tsoap);
		soap_free(tsoap);
}

/*Función encargada de una consulta Modbus*/
int ns__mb_master_consulta(struct soap *soap, struct mb_pdu_decimal *pregunta, struct mb_pdu_decimal *respuesta ){

	//Reservo memoria para datos devueltos
	respuesta->datos.__ptr=(unsigned char*)soap_malloc(soap,MB_MAX_BYTE*sizeof(unsigned char));
	//respuesta->datos.__ptr=(unsigned char*)pvPortMalloc(10*sizeof(unsigned char));
	//Envío consulta a la tarea ModbusMaster para procesar
	xQueueSend(xQueueModbusPDU, pregunta, 0);
	//Depierto la tarea ModbusMaster
	xSemaphoreGive( xSemaphoreHandleSinc);
	//Espero a que la tarea ModbusMaster procece petición y la devuelvo al clinete web
	xQueueReceive(xQueueModbusPDU, respuesta, portMAX_DELAY);

	//vPortFree(respuesta->datos.__ptr);
  return SOAP_OK;
}

/*Función encargada de configurar los parámetros del ModbusMaster*/
int ns__mb_master_config(struct soap *soap, struct mb_master_config *config, uint8_t *error){

	//Envío configuración a la tarea ModbusMaster para procesar
	xQueueSend(xQueueModbusConfigReq, config, 0);
	//Depierto la tarea ModbusMaster
	xSemaphoreGive( xSemaphoreHandleSinc);
	//Espero a que la tarea ModbusMaster procece la configuración
	//y le informo al cliente web si hubo error o la configuración fue exitosa
	xQueueReceive(xQueueModbusConfigRes, error, portMAX_DELAY);

	return SOAP_OK;
}


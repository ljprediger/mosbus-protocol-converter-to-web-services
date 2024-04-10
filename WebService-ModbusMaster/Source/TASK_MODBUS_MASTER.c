/*
 * TASK_MODBUS_MASTER.c
 *
 *  Created on: 26/05/2015
 *      Author: Leandro
 */

/*Include principal*/
#include "main.h"

/*Include Modbus*/
#include "Lib_Modbus.h"
#include "uart.h"

/*Tarea encargada del Modbus Master*/
void pxTareaModbusMaster ( void *pvParameters )
{
	//Inicializo modbus master(UART y parametros de modbus)
	mb_master_init();
	//Definición de variables
	mb_pdu_decimal pregunta;
	mb_pdu_decimal respuesta;
	mb_master_config config;
	uint8_t error=0;
	//Reservo memoria para el arreglo dinamico de datos modbus
	respuesta.datos.__ptr=(unsigned char*)pvPortMalloc(MB_MAX_BYTE*sizeof(unsigned char));
	xSemaphoreTake( xSemaphoreHandleSinc, (portTickType)1);

	while(1){

		/*Utilizamos semásforo binario para sincronización de tareas
		Seprocesa una consulta cuando el cliente realiza una petición*/
		if(xSemaphoreTake( xSemaphoreHandleSinc, (portTickType)2000)==pdTRUE){

			//Consultamos si se realizo consulta modbus
			if(xQueueReceive(xQueueModbusConfigReq, &config, 0)==pdTRUE){
				//Verificamos que parámetros de configuración sean correctos
				error=mb_master_check_config(config);
				/*Si los parámetros son correctos realizamos la configuración
				//Sino devolvemos el error*/
				if(error==0)
					mb_master_configure(config);
				//Devolvemos el error o configación exitosa al WebService
				xQueueSend(xQueueModbusConfigRes, &error, 0);
			}

			//Consultamos si se realizó una configuración modbus

			if(xQueueReceive(xQueueModbusPDU, &pregunta, 0)==pdTRUE){

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

				//Devolvemos la reaspuesta de la petición al SebService
				xQueueSend(xQueueModbusPDU, &respuesta, 0);
			}
		}
	}
}

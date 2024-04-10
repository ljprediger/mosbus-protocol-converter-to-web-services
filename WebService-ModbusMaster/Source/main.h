/*
 * main.h
 *
 *  Created on: 27/05/2015
 *      Author: Leandro
 */

#ifndef MAIN_H_
#define MAIN_H_

/* Standard includes. */
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/*Include Gsoap*/
#include "soapH.h"

/*Definición de máximo tamaño de datos en paquete modbus*/
#define MB_MAX_BYTE 20

/*Definimos tamaño de las colas de mensajes*/
#define LENGTH_QUEUE_MODBUS_PDU		2
#define LENGTH_QUEUE_MODBUS_Config	2

/*Header de colas y semáforos*/
xQueueHandle xQueueModbusPDU;
xQueueHandle xQueueModbusConfigReq;
xQueueHandle xQueueModbusConfigRes;
xSemaphoreHandle xSemaphoreHandleSinc;

/*Definiciones de las prioridades y stack de cada tarea*/
#define STACK_SIZE_WEB_SERVICE 			(size_t)((6360/4))
//#define STACK_SIZE_WEB_SERVICE 			( configMINIMAL_STACK_SIZE * 19UL )
#define	PRIORITY_TASK_WEB_SERVICE		(2U)
#define STACK_SIZE_MODBUS_MASTER		( configMINIMAL_STACK_SIZE * 2UL )
#define	PRIORITY_TASK_MODBUS_MASTER		(3U)


#endif /* MAIN_H_ */

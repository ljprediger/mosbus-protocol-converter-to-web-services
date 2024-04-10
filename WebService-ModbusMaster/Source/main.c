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

/******************************************************************************
 * The FreeRTOS+IO and FreeRTOS+CLI features mentioned in these comments are
 * documented on the following two URLs:
 * http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_IO/
 * http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_CLI/
 *
 * It is essential that jumpers are set correctly.  The required jumper
 * settings are pictured on the documentation page for this demo, which
 * can be located from the following URL:
 * http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_IO/Featured-FreeRTOS_Plus_IO_Demos.shtml
 ******************************************************************************
 *
 *
 * >>>>> THE SD CARD MUST BE INSERTED FOR THIS DEMO TO EXECUTE! <<<<<
 *
 *
 * This demo provides the following functionality:
 *
 * --- SD Card MMC Driver for FAT File System ---
 * A FAT compatible file file system is hosted on an SD card that uses an SPI
 * interface. The FreeRTOS+IO API is used with an SSP peripheral to provide the
 * necessary input and output. FatFS is used to provide the file system
 * functionality.
 *
 * The example demonstrates files being written to and read from the SD card
 * using all the available FreeRTOS+IO transfer modes. The file name is used to
 * indicate the mode being used when the file was created, and 20 files are
 * created using each mode.
 *
 * The files can be listed and manipulated using FreeRTOS+CLI - see below.
 *
 *
 * --- Telnet "like" Command Console ---
 * FreeRTOS+CLI creates a command console on the standard telnet TCP/IP port
 * (port 23). The lwIP sockets API is used to provide the TCP/IP
 * implementation.
 *
 * The command console uses the telnet port number, and can be accessed using a
 * standard telnet client, but it is not a full telnet server.
 *
 * The example uses a static IP address that is configured using the
 * configIP_ADDR0-configIP_ADDR3 constants defined in FreeRTOSConfig.h.
 *
 * Five commands are registered with FreeRTOS+CLI. These are dir, copy and del
 * to manipulate the file system (see above), and run-time-stats and task-stats
 * to view FreeRTOS task information.  See FreeRTOS-Plus-Demo-1 for example CLI 
 * command definitions that demonstrate parsing a variable number of 
 * parameteres.
 *
 * Enter "dir" in the command console to see a file system directory listing.
 *
 * Enter "del <filename>" in the command console to delete a file from the SD
 * card.
 *
 * Enter "copy <source_file> <destination_file>" in the command console to copy
 * a file.
 *
 * Enter "run-time-stats" in the command console to see the amount of time each
 * task has spent in the Running state since it was created.
 *
 * Enter "task-stats" in the command console to see a snapshot of task state
 * information - including stack high water mark data.
 *
 *
 * --- A web Server ---
 * The lwIP raw API is used to create a simple web server. The web server uses
 * server side includes (SSI) to display task state and run time information.
 *
 *
 * --- GPIO ---
 * FreeRTOS software timers are used to periodically toggle the multi-coloured
 * LEDs.
 */

//Include principal
#include "main.h"

/* FreeRTOS+IO includes. */
#include "FreeRTOS_IO.h"

/* Library includes. */
#include "LPC17xx.h"
#include "lpc17xx_emac.h"
#include "lpc17xx_gpio.h"

/* lwIP includes. */
#include "lwip/tcpip.h"

static void pxTareaWebService ( void *pvParameters );
static void pxTareaModbusMaster ( void *pvParameters );


/* The libraries use large data arrays.  Place these manually in the AHB RAM
so their combined size is not taken into account when calculating the total
Flash+RAM size of the generated executable. */
static void prvManuallyPlaceLargeDataInAHBRAM( void );

/* Defined in lwIPApps.c. */
extern void lwIPAppsInit( void *pvArguments );

/* Prepare the hardware ready to run this example. */
static void prvSetupHardware( void );

/* Used in the run time stats calculations. */
static uint32_t ulClocksPer10thOfAMilliSecond = 0UL;

/* The libraries use large data arrays.  Place these manually in the AHB RAM
so their combined size is not taken into account when calculating the total
flash+RAM size of the generated executable. */
uint8_t *pucFreeRTOSHeap = NULL;

/* The emac buffers are manually placed at the start of the AHB RAM.  These
variables store the calculated addresses, which are then made available
through the pvApplicationGetEMACTxBufferAddress() and
pvApplicationGetEMACRxBufferAddress() access functions. */
static uint32_t ulEMACTxBufferStart = 0UL, ulEMACRxBufferStart = 0UL;

/*-----------------------------------------------------------*/

int main( void )
{
	/* Configure any generic parameters needed to run the demo. */
	prvSetupHardware();

	//Creamos la cola
	xQueueModbusPDU 		= xQueueCreate( LENGTH_QUEUE_MODBUS_PDU,		sizeof(struct mb_pdu_decimal)	);
	xQueueModbusConfigReq 	= xQueueCreate( LENGTH_QUEUE_MODBUS_Config,		sizeof(struct mb_master_config));
	xQueueModbusConfigRes	= xQueueCreate( LENGTH_QUEUE_MODBUS_Config,		sizeof(uint8_t)			);
	vSemaphoreCreateBinary( xSemaphoreHandleSinc);


	/* This call creates the TCP/IP thread. */
	tcpip_init( lwIPAppsInit, NULL );

	/*Creación de tareas de WebService y ModbusMaster*/
	xTaskCreate( pxTareaWebService, ( signed char * ) "WS-Gsoap", STACK_SIZE_WEB_SERVICE, NULL, PRIORITY_TASK_WEB_SERVICE, NULL );
	xTaskCreate( pxTareaModbusMaster, ( signed char * ) "MB-Master",STACK_SIZE_MODBUS_MASTER, NULL, PRIORITY_TASK_MODBUS_MASTER, NULL );

	/* Start the FreeRTOS scheduler. */
	vTaskStartScheduler();

	/* The following line should never execute.  If it does, it means there was
	insufficient FreeRTOS heap memory available to create the Idle and/or timer
	tasks.  See the memory management section on the http://www.FreeRTOS.org web
	site for more information. */
	for( ;; );
}

void vMainConfigureTimerForRunTimeStats( void )
{
	/* How many clocks are there per tenth of a millisecond? */
	ulClocksPer10thOfAMilliSecond = configCPU_CLOCK_HZ / 10000UL;
}
/*-----------------------------------------------------------*/

uint32_t ulMainGetRunTimeCounterValue( void )
{
uint32_t ulSysTickCounts, ulTickCount, ulReturn;
const uint32_t ulSysTickReloadValue = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
volatile uint32_t * const pulCurrentSysTickCount = ( ( volatile uint32_t *) 0xe000e018 );
volatile uint32_t * const pulInterruptCTRLState = ( ( volatile uint32_t *) 0xe000ed04 );
const uint32_t ulSysTickPendingBit = 0x04000000UL;

	/* NOTE: There are potentially race conditions here.  However, it is used
	anyway to keep the examples simple, and to avoid reliance on a separate
	timer peripheral. */


	/* The SysTick is a down counter.  How many clocks have passed since it was
	last reloaded? */
	ulSysTickCounts = ulSysTickReloadValue - *pulCurrentSysTickCount;

	/* How many times has it overflowed? */
	ulTickCount = xTaskGetTickCountFromISR();

	/* This is called from the context switch, so will be called from a
	critical section.  xTaskGetTickCountFromISR() contains its own critical
	section, and the ISR safe critical sections are not designed to nest,
	so reset the critical section. */
	ulReturn = portSET_INTERRUPT_MASK_FROM_ISR();
	( void ) ulReturn;

	/* Is there a SysTick interrupt pending? */
	if( ( *pulInterruptCTRLState & ulSysTickPendingBit ) != 0UL )
	{
		/* There is a SysTick interrupt pending, so the SysTick has overflowed
		but the tick count not yet incremented. */
		ulTickCount++;

		/* Read the SysTick again, as the overflow might have occurred since
		it was read last. */
		ulSysTickCounts = ulSysTickReloadValue - *pulCurrentSysTickCount;
	}

	/* Convert the tick count into tenths of a millisecond.  THIS ASSUMES
	configTICK_RATE_HZ is 1000! */
	ulReturn = ( ulTickCount * 10UL ) ;

	/* Add on the number of tenths of a millisecond that have passed since the
	tick count last got updated. */
	ulReturn += ( ulSysTickCounts / ulClocksPer10thOfAMilliSecond );

	return ulReturn;
}
/*-----------------------------------------------------------*/

static void prvManuallyPlaceLargeDataInAHBRAM( void )
{
uint32_t ulAddressCalc;
extern uint32_t __top_RamAHB32[];
const uint32_t ulBaseAddress = 0x2007c000UL;

	/* Start at the bottom of the second bank of RAM.  Need to use a linker
	variable here! */
	ulAddressCalc = ulBaseAddress;
	ulEMACTxBufferStart = ulAddressCalc;

	/* Move up far enough to hold all the Tx buffers. */
	ulAddressCalc += sizeof( xEMACTxBuffer_t );

	/* Align and store the calculated address. */
	ulAddressCalc += 0x0fUL;
	ulAddressCalc &= ~0x0fUL;
	ulEMACRxBufferStart = ulAddressCalc;

	/* Move up far enough to hold all the Rx buffers. */
	ulAddressCalc += sizeof( xEMACRxBuffer_t );

	/* Align and assign the calculated address to the FreeRTOS heap. */
	ulAddressCalc += 0x0fUL;
	ulAddressCalc &= ~0x0fUL;
	pucFreeRTOSHeap = ( uint8_t * ) ulAddressCalc;

	/* Sanity check that the variables placed in AHB RAM actually fit in AHB
	RAM before zeroing down the memory. */
	configASSERT( ( ulAddressCalc + configTOTAL_HEAP_SIZE ) < ( uint32_t ) __top_RamAHB32 );
	memset( ( void * ) ulBaseAddress, 0x00, ( ulAddressCalc + configTOTAL_HEAP_SIZE ) - ulBaseAddress );

	/* Remove compiler warnings when configASSERT() is not defined. */
	( void ) __top_RamAHB32;
}
/*-----------------------------------------------------------*/

void *pvApplicationGetEMACTxBufferAddress( void )
{
	/* Simply return the address calculated as being the bottom of the emac
	Tx buffers. */
	return ( void * ) ulEMACTxBufferStart;
}
/*-----------------------------------------------------------*/

void *pvApplicationGetEMACRxBufferAddress( void )
{
	/* Simply return the address calculated as being the bottom of the emac
	Rx buffers. */
	return ( void * ) ulEMACRxBufferStart;
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* The examples assume that all priority bits are assigned as preemption
	priority bits. */
	NVIC_SetPriorityGrouping( 0UL );

	/* Force some large data structures into AHB RAM to keep the calculated
	flash+RAM code size down. */
	prvManuallyPlaceLargeDataInAHBRAM();

	/* The 7-segment and OLED displays are not used, but are attached to an
	SSP bus.  Ensure they are not selected (although this might not be
	necessary if they are a different bus - check the schematic). */
	//GPIO_SetDir( board7SEG_CS_PORT, board7SEG_CS_PIN, boardGPIO_OUTPUT );
	//board7SEG_DEASSERT_CS();
	//GPIO_SetDir( boardOLED_CS_PORT, boardOLED_CS_PIN, boardGPIO_OUTPUT );
	//boardOLED_DEASSERT_CS();
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

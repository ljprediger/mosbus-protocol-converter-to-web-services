/*
 * uart_V2.c
 *
 *  Created on: 08/05/2015
 *      Author: SONY-VAIO
 */


#include "LPC17xx.h"
#include "lpc_types.h"
#include "uart.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"


volatile uint32_t UART1Status,UART3Status;
volatile uint8_t UART1TxEmpty = 1, UART3TxEmpty=1;
volatile uint8_t UARTBuffer[BUFSIZE];
volatile uint32_t UARTCount = 0;
volatile int8_t caracter;
volatile int8_t estado=0;

void mb_init_uart(uint8_t trasporte,UART_CFG_Type UARTConfigStruct){

	//Si trasporte es 0, RS232 UART3
	//Si trasporte es 1, RS485 UART1

	UART_FIFO_CFG_Type UARTFIFOConfigStruct; // defino variable tipo estructura UART_FIFO_CFG_Type
	PINSEL_CFG_Type PinCfg; // defino variable tipo estructura PINSEL_CFG_Type

	if(trasporte==0){

		// Inicializo  pin connect para UART3
		PinCfg.Funcnum = 2;     // De PINSEL0 , función 10b, para TXD3
		PinCfg.OpenDrain = 0;   //No salida open drain
		PinCfg.Pinmode = 0;     //Sin pull up
		PinCfg.Pinnum = 0;      //Pin 0 para TXD3
		PinCfg.Portnum = 0;     //Port 0
		PINSEL_ConfigPin(&PinCfg); //cargo el registro PINSEL con los valores anteriores

		PinCfg.Pinnum = 1;      //Pin 1 para RXD3, más el resto de los campos anteriores
		PINSEL_ConfigPin(&PinCfg);

		// Inicializo la UART según los parametros anteriores.
		UART_Init(LPC_UART3, &UARTConfigStruct);

		// Cargo los parametros en la estructura para la FIFO.
		// Por default: FIFO_DMAMode = DISABLE, FIFO_Level = UART_FIFO_TRGLEV0
		// FIFO_ResetRxBuf = ENABLE, FIFO_ResetTxBuf = ENABLE, FIFO_State = ENABLE
		UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

		// Inicializo la FIFO según los parametros anteriores.
		UART_FIFOConfig(LPC_UART3, &UARTFIFOConfigStruct);

		// Habilito transmision en pin TXD
		UART_TxCmd(LPC_UART3, ENABLE);

		NVIC_EnableIRQ(UART3_IRQn);

		LPC_UART3->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART3 interrupt */

	}else{

	}

}



/*****************************************************************************
** Function name:		delayMS
*****************************************************************************/

void delay_us( uint32_t us )
{
	uint32_t i;

	for (i = 0; i < ((( uint32_t )120) * us); i++ )
	{
		;
	}
}



/*****************************************************************************
** Function name:		UART1_IRQHandler
**
** Descriptions:		UART1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART1_IRQHandler (void)
{
  uint8_t IIRValue, LSRValue;
  uint8_t Dummy = Dummy;

  IIRValue = LPC_UART1->IIR;

  IIRValue >>= 1;			/* skip pending bit in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  {
	LSRValue = LPC_UART1->LSR;
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  UART1Status = LSRValue;
	  Dummy = LPC_UART1->RBR;		/* Dummy read on RX to clear
								interrupt, then bail out */
	  return;
	}
	if ( LSRValue & LSR_RDR )	/* Receive Data Ready */
	{
	  /* If no error on RLS, normal ready, save into the data buffer. */
	  /* Note: read RBR will clear the interrupt */
	  UARTBuffer[UARTCount] = LPC_UART1->RBR;
	  UARTCount++;
	  if ( UARTCount == BUFSIZE )
	  {
		UARTCount = 0;		/* buffer overflow */
	  }
	}
  }
  else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
  {
	/* Receive Data Available */
	UARTBuffer[UARTCount] = LPC_UART1->RBR;
	UARTCount++;
	if ( UARTCount == BUFSIZE )
	{
	  UARTCount = 0;		/* buffer overflow */
	}
  }
  else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
  {
	/* Character Time-out indicator */
	UART1Status |= 0x100;		/* Bit 9 as the CTI error */
  }
  else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
  {
	/* THRE interrupt */
	LSRValue = LPC_UART1->LSR;		/* Check status in the LSR to see if
								valid data in U0THR or not */
	if ( LSRValue & LSR_THRE )
	{
	  UART1TxEmpty = 1;
	}
	else
	{
	  UART1TxEmpty = 0;
	}
  }

}
/*****************************************************************************
** Function name:		UART3_IRQHandler
**
** Descriptions:		UART3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void UART3_IRQHandler (void)
{
  uint8_t IIRValue, LSRValue;
  uint8_t Dummy = Dummy;

  IIRValue = LPC_UART3->IIR;

  IIRValue >>= 1;			/* skip pending bit in IIR */
  IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
  if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  {
	LSRValue = LPC_UART3->LSR;
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
	{
	  /* There are errors or break interrupt */
	  /* Read LSR will clear the interrupt */
	  UART3Status = LSRValue;
	  Dummy = LPC_UART3->RBR;		/* Dummy read on RX to clear
							interrupt, then bail out */
	  return;
	}
	if ( LSRValue & LSR_RDR )	/* Receive Data Ready */
	{
	  /* If no error on RLS, normal ready, save into the data buffer. */
	  /* Note: read RBR will clear the interrupt */
	  UARTBuffer[UARTCount] = LPC_UART3->RBR;
	  UARTCount++;
	  if ( UARTCount == BUFSIZE )
	  {
		UARTCount = 0;		/* buffer overflow */
	  }
	}
  }
  else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
  {
	/* Receive Data Available */
	UARTBuffer[UARTCount] = LPC_UART3->RBR;
	UARTCount++;
	if ( UARTCount == BUFSIZE )
	{
	  UARTCount = 0;		/* buffer overflow */
	}
  }
  else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
  {
	/* Character Time-out indicator */
	UART3Status |= 0x100;		/* Bit 9 as the CTI error */
  }
  else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
  {
	/* THRE interrupt */
	LSRValue = LPC_UART3->LSR;		/* Check status in the LSR to see if
									valid data in U0THR or not */
	if ( LSRValue & LSR_THRE )
	{
	  UART3TxEmpty = 1;
	}
	else
	{
	  UART3TxEmpty = 0;
	}
  }
}

/*****************************************************************************
** Function name:		UARTSend
**
** Descriptions:		Send a block of data to the UART 0 port based
**						on the data length
**
** parameters:			portNum, buffer pointer, and data length
** Returned value:		None
**
*****************************************************************************/
void UARTSend( uint8_t trasporte, uint8_t *BufferPtr, uint32_t Length )
{
	if (trasporte==0){
	    while ( Length != 0 ){
		  /* THRE status, contain valid data */
		  while ( !(UART3TxEmpty & 0x01) );
		  LPC_UART3->THR = *BufferPtr;
		  UART3TxEmpty = 0;	/* not empty in the THR until it shifts out */
		  BufferPtr++;
		  Length--;
		}
	  }
	if (trasporte==1){
		while ( Length != 0 ){
			/* THRE status, contain valid data */
			while ( !(UART1TxEmpty & 0x01) );
			LPC_UART1->THR = *BufferPtr;
			UART1TxEmpty = 0;	/* not empty in the THR until it shifts out */
			BufferPtr++;
			Length--;
		}
	}
  return;
}



/*****************************************************************************
** Function name:		UART1Scan
*****************************************************************************/

uint16_t UARTScan (uint8_t trasporte, int8_t *Rx , uint16_t timeout)
{
	uint16_t newCount = 0;
	uint16_t oldCount;

	UARTCount = 0;

	if(trasporte==0){

		LPC_UART3->IER = IER_THRE | IER_RLS | IER_RBR; // enable interrupt RX UART3
		//delayMS (timeout); // wait sometime for receive some commands
		vTaskDelay(timeout);
		do{
			oldCount = newCount;
			//delayMS (5);
			vTaskDelay(5);
			newCount = UARTCount;
		} while (oldCount != newCount); // wait a little time while we have new char

		LPC_UART3->IER = IER_THRE | IER_RLS; // disable interrupt RX UART3

	}

	if(trasporte==1){

		LPC_UART1->IER = IER_THRE | IER_RLS | IER_RBR; // enable interrupt RX UART0
		//delayMS (timeout); // wait sometime for receive some commands
		vTaskDelay(timeout);
		do{
			oldCount = newCount;
			//delayMS (10);
			vTaskDelay(5);
			newCount = UARTCount;
		} while (oldCount != newCount); // wait a little time while we have new char

		LPC_UART1->IER = IER_THRE | IER_RLS; // disable interrupt RX UART0

	}

	strncpy (Rx, (int8_t *) UARTBuffer, newCount);
	Rx[newCount] = 0;	// add end string. Now we have a string

	//limpiamos la uart
	UARTCount = 0;
	memset((int8_t *)UARTBuffer, 0x00, BUFSIZE);

	return newCount;


}



/******************************************************************************
**                            End Of File
******************************************************************************/

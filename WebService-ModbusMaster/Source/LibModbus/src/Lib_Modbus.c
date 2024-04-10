/*
 * lib_modbus_V2.c
 *
 *  Created on: 16/2/2015
 *      Author: Leandro
 */

/* Includes */
#include "Lib_Modbus.h"

/* Variables */
static uint8_t mb_master_reintentos;		//Catidad de reintentos si existe error en petición modbus
static uint8_t mb_master_time_out;			//Tiempo máximo de espera para una respuesta modbus
static uint8_t mb_master_trasporte;			//0 RS232 UART3- 1 RS485 UART1

/*
 * *********************************************************************************************
 *
 * Nombre: hex2ascii_a_decimal
 *
 * Descripción: Trasforma dos caracteres hexadecimales en un número decimal
 *
 * Entrada: Puntero a dos caracteres ASII-HEX
 *
 * Salida: Número decimal
 *
 ************************************************************************************************
 */
uint8_t hex2ascii_a_decimal(uint8_t*string_hex){
	uint8_t aux[3];			//Variable auxiliar
	aux[2]='\0';			//El último (\0) identifica fin
	strncpy(aux, string_hex, 2);				//Copia dos primeros caracteres en variable auxiliar
	return (uint8_t)strtol(aux, NULL, 16);		//Trasforma los dos caracteres hex a decimal
}

/*
 * *********************************************************************************************
 *
 * Nombre: mb_calc_LRC
 *
 * Descripción: Calcula el LRC de PDU modbus pregunta en formato decimal
 *
 * Entrada: Puntero a mb_pdu_decimal
 *
 * Salida:	Valor decimal del LRC
 *
 ************************************************************************************************
 */
uint8_t mb_calc_LRC(mb_pdu_decimal aux){
	uint8_t ret=0;		//LRC, acumulador
	uint8_t i=0;		//Variable auxiliar
	//Suma byte a byte
	ret=ret+aux.ID_escalvo;
	ret=ret+aux.funcion;
	for(i=0;i<aux.datos.__size;i++){
		ret=ret+aux.datos.__ptr[i];
	}
	//Realiza el complemento A2 t devuelve
	return (uint8_t)(256-ret);
}

/*
 * *********************************************************************************************
 *
 * Nombre: checkLRC
 *
 * Descripción: Verifica si el LRC de una respuesta modbus es correcta
 *
 * Entrada: Puntero a respuesta modbus ASCII
 * 			Longitud del arrreglo
 *
 * Salida:	Error:	0: LRC OK
 * 					1: Existe error
 *
 ************************************************************************************************
 */
uint8_t checkLRC(uint8_t * mb_res_string, uint8_t len){
	uint8_t i;		//Variable auxiliar
	uint8_t LRC=0;	//Varible para almacenar suma de bytes
	uint8_t ret=1;	//Varibel de retorno, por defecto existe error

	//Calculo la suma de bytes
	for(i=0;i<(len/2)-1;i++){
		LRC+=hex2ascii_a_decimal(&mb_res_string[i*2]);
	}
	//Complemento A2
	LRC=(uint8_t)(256-LRC);

	//Si LRC OK revolvemos 0
	if(LRC==hex2ascii_a_decimal(&mb_res_string[i*2]))
		ret=0;

	return ret;
}

/*
 * *********************************************************************************************
 *
 * Nombre: mb_master_init
 *
 * Descripción: Inicializa mb_master con sus parámetros por defecto
 *
 * Entrada: No tiene
 *
 * Salida:	No tiene
 *
 ************************************************************************************************
 */
void mb_master_init(void){

	//Configuración de reintentos y time-out
	mb_master_reintentos=3;
	mb_master_time_out=100;
	mb_master_trasporte=0;

	UART_CFG_Type UARTConfigStruct;
	UARTConfigStruct.Baud_rate=57600;
	UARTConfigStruct.Databits=UART_DATABIT_7;
	UARTConfigStruct.Parity=UART_PARITY_EVEN;
	UARTConfigStruct.Stopbits=UART_STOPBIT_1;

	//inicialización por defecto UART3
	mb_init_uart(mb_master_trasporte,UARTConfigStruct);

}

/*
* *********************************************************************************************
*
* Nombre: mb_master_configure
*
* Descripción: ReConfigura mb_master con sus parámetros correspodientes
*
* Entrada: Estructura mb_master_config
*
* Salida:	No tiene
*
************************************************************************************************
*/
void mb_master_configure(mb_master_config config){

	//ReConfiguración de reintentos y time-out
	mb_master_reintentos=config.Cant_Reintentos;
	mb_master_time_out=config.Time_Out;
	mb_master_trasporte=config.Trasporte;
	UART_CFG_Type UARTConfigStruct;
	UARTConfigStruct.Baud_rate=config.Baud_Rate;
	UARTConfigStruct.Databits=config.Data_Bits;
	UARTConfigStruct.Parity=config.Parity_Bits;
	UARTConfigStruct.Stopbits=config.Stop_Bits;

	//ReConfiguracion de la UART
	mb_init_uart(mb_master_trasporte, UARTConfigStruct);
}

/*
 * *********************************************************************************************
 *
 * Nombre: mb_master_check_config
 *
 * Descripción: Verifica que los parámetros de la estructura de mb_master_config sean correctos
 *
 * Entrada: Estructura mb_master_config
 *
 * Salida:
 * 			0: No existe error
 * 			1: Error en Trasporte
 * 			2: Error en Time_Out
 * 			3: Error en Stop_Bits
 * 			4: Error en Parity_Bits
 * 			5: Error en Data_Bits
 * 			6: Error en Cant_Reintentos
 * 			7: Error en Baud_Rate
 *
 ************************************************************************************************
 */
uint8_t mb_master_check_config(mb_master_config config){

	uint8_t error=0;

	if(config.Trasporte>1 || config.Trasporte<0)
		error=1;

	if(config.Time_Out<=0)
		error=2;

	if(config.Stop_Bits<0 || config.Stop_Bits>1)
		error=3;

	if(config.Parity_Bits<0 || config.Parity_Bits>3)
		error=4;

	if(config.Data_Bits<0 || config.Data_Bits>3)
		error=5;

	if(config.Cant_Reintentos<=0)
		error=6;

	if(config.Baud_Rate!=2400 && config.Baud_Rate!=4800 && config.Baud_Rate!=9600 && config.Baud_Rate!=14400 && config.Baud_Rate!=19200 && config.Baud_Rate!=38400 && config.Baud_Rate!=57600 && config.Baud_Rate!=115200)
		error=7;

	return error;

}

/*
 * *********************************************************************************************
 *
 * Nombre: mb_master_check_preg
 *
 * Descripción: Verifica que los parámetros de la estructura de mb_pdu_decimal pregunta sean correctos
 *
 * Entrada: Estructura mb_pdu_decimal pregunta
 *
 * Salida:	enum mb_error que identifica
 * 										0: No existe error
 * 										1: Función ilegal
 * 										2: Dirección ilegal
 * 										3: Cantidad de registros pedidos ilegal
 * 										4: El esclavo falla
 *
 ************************************************************************************************
 */
mb_error mb_master_check_preg(mb_pdu_decimal pregunta){


	mb_error ret=No_Error;		//Variable de retorno error
	//Dependiendo de la función
	switch (pregunta.funcion){
	case 1:
	case 2:
	case 15:
		//Verificamos cantidad de registros a leer(1bit)
		if((pregunta.datos.__ptr[2]*256+pregunta.datos.__ptr[3])>(MB_MAX_BYTE/8)){
			ret=Cant_Reg_Ilegal;
		}
		else{
			//Verificamos que dirección de inicio + cantidad de registros no supere el máximo
			if((pregunta.datos.__ptr[2]*256+pregunta.datos.__ptr[3])+(pregunta.datos.__ptr[0]*256+pregunta.datos.__ptr[1])>MB_MAX_DIR){
				ret=Direccion_Ilegal;
			}
		}
		break;
	case 3:
	case 4:
	case 16:
		//Verificamos cantidad de registros a leer(16bit)
		if(((pregunta.datos.__ptr[2]*256+pregunta.datos.__ptr[3])*2)>MB_MAX_BYTE){
			ret=Cant_Reg_Ilegal;
		}
		else{
			//Verificamos que dirección de inicio + cantidad de registros no supere el máximo
			if((pregunta.datos.__ptr[2]*256+pregunta.datos.__ptr[3])+(pregunta.datos.__ptr[0]*256+pregunta.datos.__ptr[1])>MB_MAX_DIR){
				ret=Direccion_Ilegal;
			}
		}
		break;
	case 5:
	case 6:
		//Verificamos que la dirección sea correcta
		if((pregunta.datos.__ptr[0]*256+pregunta.datos.__ptr[1])>MB_MAX_DIR){
			ret=Direccion_Ilegal;
		}
		break;
	default:
		ret=Funcion_Ilegal;
		break;
	}
	return ret;
}

/*
 * *********************************************************************************************
 *
 * Nombre: mb_master_procesa_peticion
 *
 * Descripción: Procesa la petición modbus, envia la pregunta por la uart, lee la respuesta, y calcula el error si existe
 *
 * Entrada: 	Estructura mb_pdu_decimal pregunta
 * 				Puntero a estructura mb_pdu_decimal respuesta
 *
 * Salida:	enum mb_error que identifica
 * 										0: No existe error
 * 										1: Función ilegal
 * 										2: Dirección ilegal
 * 										3: Cantidad de registros pedidos ilegal
 * 										4: El esclavo falla
 *
 ************************************************************************************************
 */
mb_error mb_master_procesa_peticion(mb_pdu_decimal pregunta,mb_pdu_decimal* respuesta){

	uint8_t reintentos=0;		//Varibale auxiliar para cantidad de reintentos
	mb_error error=No_Error;	//Variable auxiliar de retorno

	do{
		reintentos++;							//Se incrementa con cada reenvío
		mb_master_send_pdu(pregunta);			//Calcular LRC y armar string de pregunta con Inicio y Fin y envía por la UART
		error=mb_master_read_pdu(respuesta);	//Devuelve la estructura de respuesta, o el error correspondiente
	//Realiza esto cada vez que exista error y no supere la cantidad máxima de reintentos
	}while(error!=No_Error && reintentos<=mb_master_reintentos);

	return error;
}

/*
 * *********************************************************************************************
 *
 * Nombre: mb_master_send_pdu
 *
 * Descripción: Envía mb_pdu_decimal pregunta por la UART, para ello:
 * 										1: Calcula LRC
 * 										2: Trasforma a string HEX
 * 										3: Agrega caracteres de inicio y fin
 *
 * Entrada: Estructura mb_pdu_decimal pregunta
 *
 * Salida:	No tiene
 *
 ************************************************************************************************
 */
void mb_master_send_pdu ( mb_pdu_decimal pregunta){
	uint8_t indice=0;		//Variable para armar string
	uint8_t i=0;			// Varibale auxiliar
	int8_t mb_preg_string[MB_MAX_STRING];		//Reservo memoria para string de pregunta
	mb_preg_string[indice]=':';					//Caracter de INICIO ':'
	indice++;
	sprintf(&mb_preg_string[indice],"%02X",pregunta.ID_escalvo);	//Caracteres de IDesclavo
	indice=indice+2;
	sprintf(&mb_preg_string[indice],"%02X",pregunta.funcion);		//Caracteres de función
	indice=indice+2;
	sprintf(&mb_preg_string[indice],"%04X",pregunta.datos.__ptr[0]*256+pregunta.datos.__ptr[1]);	//Caracteres de direccion de incio
	indice=indice+4;
	sprintf(&mb_preg_string[indice],"%04X",pregunta.datos.__ptr[2]*256+pregunta.datos.__ptr[3]);	//Caracteres de cantidad de registros
	indice=indice+4;
	//Caracteres de datos
	if(pregunta.datos.__size>4){
		for(i=4;i<pregunta.datos.__size;i++){
			sprintf(&mb_preg_string[indice],"%02X",pregunta.datos.__ptr[i]);
			indice=indice+2;
		}
	}
	sprintf(&mb_preg_string[indice],"%02X",mb_calc_LRC(pregunta));		//Caracteres del LRC
	indice=indice+2;
	mb_preg_string[indice]='\r';		//Caracter 1 de FIN
	indice++;
	mb_preg_string[indice]='\n';		//Caracter 2 de FIN
	indice++;
	UARTSend(mb_master_trasporte,mb_preg_string, indice*sizeof(int8_t));	//Enviamos string por la UART
}

/*
 * *********************************************************************************************
 *
 * Nombre: mb_master_read_pdu
 *
 * Descripción: Lee pdu devuelto por esclavo, calcula error si existe y arma mb_pdu_decimal respuesta
 *
 * Entrada: Puntero a estructura mb_pdu_decimal respuesta
 *
 * Salida:	enum mb_error que identifica
 * 										0: No existe error
 * 										1: Función ilegal
 * 										2: Dirección ilegal
 * 										3: Cantidad de registros pedidos ilegal
 * 										4: El esclavo falla
 *
 ************************************************************************************************
 */
mb_error mb_master_read_pdu(mb_pdu_decimal* respuesta){
	uint8_t i=0;		//Varibel auxiliar
	uint8_t LRC;		//Varibale que almacena el LRC devuelto
	int8_t mb_res_string[MB_MAX_STRING];		//Reservo memoria para string de respuesta
	mb_error error=No_Error;					//Variable de retorno
	UARTScan(mb_master_trasporte,mb_res_string,mb_master_time_out);			//Leo lo recibido por la UART

	//Busco caracteres de fin \r y calculo longitud del string
	while(mb_res_string[i]!='\r' && i<(11+MB_MAX_BYTE)){
		i++;
	}

	//Verifica que estén los caracteres de INICIO y FIN
	if(mb_res_string[0]==':' && mb_res_string[i]=='\r' && mb_res_string[i+1]=='\n' && checkLRC(&mb_res_string[1],i-1)==0){
		//Calculamos valor numerico de ID_esclavo
		respuesta->ID_escalvo=hex2ascii_a_decimal(&mb_res_string[1]);
		//Calculamos valor numerico de funcion
		respuesta->funcion=hex2ascii_a_decimal(&mb_res_string[3]);
		//Verificamos funcion
		if(respuesta->funcion<128){
			//Dependiendo de que función es
			switch(respuesta->funcion){
			case 1:
			case 2:
			case 3:
			case 4:
				respuesta->datos.__ptr[0]=hex2ascii_a_decimal(&mb_res_string[5]);		//Trasforno cantidad de bytes
				respuesta->datos.__size=1+respuesta->datos.__ptr[0];		//Calculo longitud de datos
				//Trasforno los datos
				for(i=1;i<=respuesta->datos.__ptr[0];i++){
					respuesta->datos.__ptr[i]=hex2ascii_a_decimal(&mb_res_string[5+i*2]);
				}
				break;

			case 5:
			case 6:
			case 16:
				//Trasformo los primeros 4 bytes
				for(i=0;i<=3;i++){
					respuesta->datos.__ptr[i]=hex2ascii_a_decimal(&mb_res_string[7+i*2]);
				}
				respuesta->datos.__size=4;
				break;

			case 15:
				//Trasformo los primeros 4 bytes
				for(i=0;i<=3;i++){
					respuesta->datos.__ptr[i]=hex2ascii_a_decimal(&mb_res_string[7+i*2]);
				}
				respuesta->datos.__ptr[4]=hex2ascii_a_decimal(&mb_res_string[15]);	//Trasformo 5to bytes que es cantidad de bytes enviados
				respuesta->datos.__size=5+respuesta->datos.__ptr[4];		//Calculo longitud de datos
				//Trasformo lo que falta de datos
				for(i=5;i<5+respuesta->datos.__ptr[4];i++){
					respuesta->datos.__ptr[i]=hex2ascii_a_decimal(&mb_res_string[7+i*2]);
				}
				break;

			default:
				error=Funcion_Ilegal;		// Si no es una de estas funciones devuelve error en función
				break;
			}
		}
		//Si existe error en función armamos modbus_respuesta correspondiente
		else{
			respuesta->datos.__size=1;
			respuesta->datos.__ptr[0]=hex2ascii_a_decimal(&mb_res_string[5]);
		}
	}
	//Si no están caracteres de INICIO o FIN genera un error
	else{
		error=Esclavo_Falla;
	}
	return error;
}

/*
 * *********************************************************************************************
 *
 * Nombre: mb_master_exception_error
 *
 * Descripción: Si la consulta hecha no es soportada, o existió un error en el esclavo arma la respuesta dependiendo del error
 *
 * Entrada: 	Puntero a estructura mb_pdu_decimal pregunta
 * 				Puntero a estructura mb_pdu_decimal respuesta
 *				Estructura mb_error
 * Salida:	No tiene
 *
 ************************************************************************************************
 */
void mb_master_exception_error( mb_pdu_decimal* pregunta, mb_pdu_decimal* respuesta, mb_error error){

	respuesta->ID_escalvo=pregunta->ID_escalvo;		//Iguala ID_esclavo
	//Si función de pregunta es mayor que 128
	if(pregunta->funcion>=128){
		respuesta->funcion=pregunta->funcion;
	}
	else{
		respuesta->funcion=128+pregunta->funcion;
	}
	respuesta->datos.__size=1;
	respuesta->datos.__ptr[0]=error;

}










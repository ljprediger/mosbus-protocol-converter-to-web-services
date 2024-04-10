/*
 * lib_modbus_V2.h
 *
 *  Created on: 16/2/2015
 *      Author: Leandro
 */

#ifndef LIB_MODBUS_V2_H_
#define LIB_MODBUS_V2_H_

/* Includes */
#include "stdint.h"
#include "soapH.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "uart.h"

/* Definiciones */
#define MB_MAX_BYTE 64							//Máxima cantidad de byte en el arreglo datos
#define MB_MAX_STRING	MB_MAX_BYTE+16 			//Máxima cantidad de caracteres hex para una petición
#define MB_MAX_DIR 10000						//Dirección máxima soportada: actualmente la mas alta

typedef struct mb_pdu_decimal mb_pdu_decimal;
typedef struct mb_master_config mb_master_config;

/**
 * 	Identifica el error modbus
 */
typedef enum {
	No_Error 			=	0,
	Funcion_Ilegal 		=	1,
	Direccion_Ilegal 	=	2,
	Cant_Reg_Ilegal 	= 	3,
	Esclavo_Falla		=	4
} mb_error;


uint8_t hex2ascii_a_decimal(uint8_t*string_hex);

uint8_t checkLRC(uint8_t * mb_res_string, uint8_t len);

uint8_t mb_calc_LRC(mb_pdu_decimal aux);

void mb_master_init(void);

void mb_master_configure(mb_master_config config);

uint8_t mb_master_check_config(mb_master_config config);

mb_error mb_master_check_preg(mb_pdu_decimal pregunta);

mb_error mb_master_procesa_peticion(mb_pdu_decimal pregunta,mb_pdu_decimal* respuesta);

void mb_master_send_pdu ( mb_pdu_decimal pregunta);

mb_error mb_master_read_pdu(mb_pdu_decimal* respuesta);

void mb_master_exception_error( mb_pdu_decimal* pregunta, mb_pdu_decimal* respuesta, mb_error error);



#endif /* LIB_MODBUS_V2_H_ */

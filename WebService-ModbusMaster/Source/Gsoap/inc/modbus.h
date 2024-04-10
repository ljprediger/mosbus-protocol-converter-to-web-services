/*
	modbus.h
	
	Compilation in C ):
	$ soapcpp2 -c modbus.h

	Note that soapcpp2 option -i generates proxy and service classes, which
	encapsulate the method operations in the class instead of defining them
	as global functions as in C. 

	The //gsoap directives are used to bind XML namespaces and to define
	Web service properties:

	//gsoap <ns> service name: <WSDLserviceName> <documentationText>
	//gsoap <ns> service style: [rpc|document]
	//gsoap <ns> service encoding: [literal|encoded]
	//gsoap <ns> service namespace: <WSDLnamespaceURI>
	//gsoap <ns> service location: <WSDLserviceAddressLocationURI>

	Web service operation properties:

	//gsoap <ns> service method-style: <methodName> [rpc|document]
	//gsoap <ns> service method-encoding: <methodName> [literal|encoded]
	//gsoap <ns> service method-action: <methodName> <actionString>
	//gsoap <ns> service method-documentation: <methodName> <documentation>

	and type schema properties:

	//gsoap <ns> schema namespace: <schemaNamespaceURI>
	//gsoap <ns> schema elementForm: [qualified|unqualified]
	//gsoap <ns> schema attributeForm: [qualified|unqualified]
	//gsoap <ns> schema documentation: <documentationText>
	//gsoap <ns> schema type-documentation: <typeName> <documentationText>

	where <ns> is an XML namespace prefix, which is used in C/C++ operation
	names, e.g. ns__add(), and type names, e.g. xsd__int.

	See the documentation for the full list of //gsoap directives.

--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2001-2008, Robert van Engelen, Genivia, Inc. All Rights Reserved.
This software is released under one of the following two licenses:
GPL or Genivia's license for commercial use.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/


//gsoap ns service name:		modbus master
//gsoap ns service style:		rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	urn:modbus
//gsoap ns service location:	http://192.168.0.200:24
//gsoap ns schema namespace:	urn:modbus


/**
*	Arreglo de dimencion variable para datos modbus.
*/
struct ARREGLO_type{
	unsigned char* __ptr;			/*!Puntero al arreglo de bytes*/
	int __size;						/*!Longitud del arreglo*/
};

/**
*	Estructura de peticion, tanto de pregunta como de respuesta.
*/
struct mb_pdu_decimal{
	unsigned char				ID_escalvo;		/*!Dirección del esclavo modbus*/
	unsigned char				funcion;		/*!Código de la función modbus a realizar*/
	struct ARREGLO_type			datos;			/*!Datos necesarios para cada petición modbus*/
};

/**
*	Estructura de configuración de modbus master
*/
struct  mb_master_config{
	unsigned char			Trasporte;				/*! 0 es RS323 y 1 es RS485 */
	unsigned char 			Cant_Reintentos;		/*! Cantidad de reintentos de modbus master */
	unsigned short			Time_Out;				/*! Tiempo de espera para respeusta modbus */
	unsigned long 			Baud_Rate;				/*! Velocidad de trasmision de la UART */
	unsigned char			Data_Bits;				/*! Catidad de bits de datos de la UART */
	unsigned char			Parity_Bits;			/*! Cantidad de bits de stop de la UART */
	unsigned char			Stop_Bits;				/*! Tipo de paridad de la UART */
};

/**
*	mb_error_config:
*	Identifica si se pudo realizar la configuración con éxito
 * 			0: No existe error
 * 			1: Error en Trasporte
 * 			2: Error en Time_Out
 * 			3: Error en Stop_Bits
 * 			4: Error en Parity_Bits
 * 			5: Error en Data_Bits
 * 			6: Error en Cant_Reintentos
 * 			7: Error en Baud_Rate
*/


//gsoap ns service method-documentation: realiza una consulta modbus a traves de web services
int ns__mb_master_consulta( struct mb_pdu_decimal *pregunta, struct mb_pdu_decimal *respuesta);

//gsoap ns service method-documentation: configura los parámetros de modbus master
int ns__mb_master_config( struct mb_master_config *config, unsigned char *mb_error_config);



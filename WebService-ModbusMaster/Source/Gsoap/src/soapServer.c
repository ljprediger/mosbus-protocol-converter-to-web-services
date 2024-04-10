/* soapServer.c
   Generated by gSOAP 2.7.14 from modbus.h
   Copyright(C) 2000-2009, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"
#ifdef __cplusplus
extern "C" {
#endif

SOAP_SOURCE_STAMP("@(#) soapServer.c ver 2.7.14 2015-05-25 19:09:32 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap *soap)
{
#ifndef WITH_FASTCGI
	unsigned int k = soap->max_keep_alive;
#endif

	do
	{
#ifdef WITH_FASTCGI
		if (FCGI_Accept() < 0)
		{
			soap->error = SOAP_EOF;
			return soap_send_fault(soap);
		}
#endif

		soap_begin(soap);

#ifndef WITH_FASTCGI
		if (soap->max_keep_alive > 0 && !--k)
			soap->keep_alive = 0;
#endif

		if (soap_begin_recv(soap))
		{	if (soap->error < SOAP_STOP)
			{
#ifdef WITH_FASTCGI
				soap_send_fault(soap);
#else 
				return soap_send_fault(soap);
#endif
			}
			soap_closesock(soap);

			continue;
		}

		if (soap_envelope_begin_in(soap)
		 || soap_recv_header(soap)
		 || soap_body_begin_in(soap)
		 || soap_serve_request(soap)
		 || (soap->fserveloop && soap->fserveloop(soap)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);
#else
			return soap_send_fault(soap);
#endif
		}

#ifdef WITH_FASTCGI
		soap_destroy(soap);
		soap_end(soap);
	} while (1);
#else
	} while (soap->keep_alive);
#endif
	return SOAP_OK;
}

#ifndef WITH_NOSERVEREQUEST
SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap *soap)
{
	if (soap_peek_element(soap))
	return soap->error;
	if (!soap_match_tag(soap, soap->tag, "ns:mb-master-consulta"))
		return soap_serve_ns__mb_master_consulta(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:mb-master-config"))
		return soap_serve_ns__mb_master_config(soap);
	return soap->error = SOAP_NO_METHOD;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__mb_master_consulta(struct soap *soap)
{	struct ns__mb_master_consulta soap_tmp_ns__mb_master_consulta;
	struct mb_pdu_decimal respuesta;
	soap_default_mb_pdu_decimal(soap, &respuesta);
	soap_default_ns__mb_master_consulta(soap, &soap_tmp_ns__mb_master_consulta);
	soap->encodingStyle = "";
	if (!soap_get_ns__mb_master_consulta(soap, &soap_tmp_ns__mb_master_consulta, "ns:mb-master-consulta", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__mb_master_consulta(soap, soap_tmp_ns__mb_master_consulta.pregunta, &respuesta);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_mb_pdu_decimal(soap, &respuesta);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_mb_pdu_decimal(soap, &respuesta, "mb-pdu-decimal", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_mb_pdu_decimal(soap, &respuesta, "mb-pdu-decimal", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__mb_master_config(struct soap *soap)
{	struct ns__mb_master_config soap_tmp_ns__mb_master_config;
	struct ns__mb_master_configResponse soap_tmp_ns__mb_master_configResponse;
	unsigned char soap_tmp_unsignedByte;
	soap_default_ns__mb_master_configResponse(soap, &soap_tmp_ns__mb_master_configResponse);
	soap_default_unsignedByte(soap, &soap_tmp_unsignedByte);
	soap_tmp_ns__mb_master_configResponse.mb_error_config = &soap_tmp_unsignedByte;
	soap_default_ns__mb_master_config(soap, &soap_tmp_ns__mb_master_config);
	soap->encodingStyle = "";
	if (!soap_get_ns__mb_master_config(soap, &soap_tmp_ns__mb_master_config, "ns:mb-master-config", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__mb_master_config(soap, soap_tmp_ns__mb_master_config.config, &soap_tmp_unsignedByte);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_ns__mb_master_configResponse(soap, &soap_tmp_ns__mb_master_configResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__mb_master_configResponse(soap, &soap_tmp_ns__mb_master_configResponse, "ns:mb-master-configResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__mb_master_configResponse(soap, &soap_tmp_ns__mb_master_configResponse, "ns:mb-master-configResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapServer.c */
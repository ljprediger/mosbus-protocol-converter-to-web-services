
#ifndef GSOAP_LWIP_IO_H_
#define GSOAP_LWIP_IO_H_

#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "lwip/opt.h"

#include "lwip/init.h"

#include "lwip/api.h"

#include "lwip/tcpip.h"
#include "lwip/sockets.h"

#include "lwip/ip_addr.h"

#include "stdsoap2.h"

int gsoap_lwip_init (struct netif *netifp);
int gsoap_lwip_register_handle (struct soap *soap);
SOAP_SOCKET soap_bind (struct soap * soap, const char *host, int port, int backlog);
SOAP_SOCKET soap_accept (struct soap * soap);

#endif /* GSOAP_LWIP_IO_H_ */

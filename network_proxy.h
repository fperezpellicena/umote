#ifndef NETWORK_PROXY_H
#define	NETWORK_PROXY_H

#include "bsp.h"

BOOL NetworkAwake(void);

void ClearNetworkInterrupt(void);

void SendMeasures(void);

#endif	/* NETWORK_PROXY_H */


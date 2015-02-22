#ifndef REGISTER_H
#define	REGISTER_H

#include "bsp.h"

void RegisterLockIO(void);

void RegisterUnLockIO(void);

void RegisterRemap(far uint8_t* rpin, uint8_t func);

#endif	/* REGISTER_H */


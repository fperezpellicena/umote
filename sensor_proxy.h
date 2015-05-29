#ifndef SENSOR_PROXY_H
#define	SENSOR_PROXY_H

#include "bsp.h"

void SensorProxyInit(void);

void MeasureSensors(void);

void EnableCO2Sensor(void);

void StartCO2SensorDelayed(void);

BOOL MustEnableCO2Sensor(void);

BOOL MustToggleLampPulse(void);

void ToggleLampPulse(void);

#endif	/* SENSOR_PROXY_H */


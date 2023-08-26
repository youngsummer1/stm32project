#ifndef __GPS_H
#define __GPS_H
void GPS_Init();
uint8_t GPS_GetRxFlag(void);
double GPS_GetRxDataN(void);
double GPS_GetRxDataE(void);

#endif
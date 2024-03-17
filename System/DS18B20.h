#ifndef _DS18B20_H
#define _DS18B20_H

extern unsigned int sdata;            //温度的部分
extern unsigned char fg;                    //温度正负标志

void Init_DS18B20(void);
void ReadSoilTemperature(void);

#endif

#ifndef _DS18B20_H
#define _DS18B20_H

extern unsigned int sdata;            //�¶ȵĲ���
extern unsigned char fg;                    //�¶�������־

void Init_DS18B20(void);
void ReadSoilTemperature(void);

#endif

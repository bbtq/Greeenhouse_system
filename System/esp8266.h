#ifndef _ESP8266_H
#define _ESP8266_H
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
	void ESP8266_Init(void);
    uint8_t MQTT_TX_messege(u16 now_temp,u16 now_humi,u16 now_soiltemp,u16 now_soilhumi);
    void MQTT_RX_messege(uint8_t * app_flag);
#endif

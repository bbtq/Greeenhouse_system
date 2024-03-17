#include "esp8266.h"
#include "usart.h"
#include "stm32f10x_usart.h"
#include "Delay.h"
#include <stdio.h>
#include <string.h>
#include "esp8266.h"
#include "OLED.h"
void ESP8266_Init(void)
{
    int timeout = 200;
    Delay_ms(2000);
    if(!(strstr((const char*)Serial_RxPacket,"ready"))) while(timeout--){Delay_ms(10);}
        buf_clear();
	//重启ESP
	Serial_Printf("AT+RESTORE\r\n");
    OLED_ShowString(3,5,"10%");
		//Delay_ms(1000);
        Delay_ms(5000);
    timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);}
        buf_clear();
	//wifi模式设置
	Serial_Printf("AT+CWMODE=1\r\n");
    OLED_ShowString(3,5,"25%");
		//Delay_ms(1000);
    timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);};
        buf_clear();
	//服务器设置
	Serial_Printf("AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n");		
    OLED_ShowString(3,5,"35%");
		//Delay_ms(1000);
    timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);};
        buf_clear();
	//wifi名称，WiFi密码
	Serial_Printf("AT+CWJAP=\"huanhuan\",\"88888888\"\r\n");
    OLED_ShowString(3,5,"45%");
        Delay_ms(1000);
    timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);};
        buf_clear();
        Delay_ms(500);
	//MQTT名称，MQTT密码
	Serial_Printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"esp_01s_bishe1&k0t4jeVASxd\",\"daa3cd5fc9b56b5171d7163873f5094199febd19860043628d6d7407e91f0ddd\",0,0,\"\"\r\n");
    OLED_ShowString(3,5,"55%");
		//Delay_ms(1000);
    timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);};
        buf_clear();
	//MQTTID
        Delay_ms(500);
	Serial_Printf("AT+MQTTCLIENTID=0,\"k0t4jeVASxd.esp_01s_bishe1|securemode=2\\,signmethod=hmacsha256\\,timestamp=1709702939347|\"\r\n");
    OLED_ShowString(3,5,"65%");
//		Delay_ms(1000);
    timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);};
        buf_clear();
	//MQTT域名
        Serial_Printf("AT+MQTTCONN=0,\"iot-06z00fayetowk6o.mqtt.iothub.aliyuncs.com\",1883,1\r\n");
    OLED_ShowString(3,5,"75%");
        Delay_ms(5000);
        timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);};
        buf_clear();
        
        Serial_Printf("AT+MQTTCONN=0,\"iot-06z00fayetowk6o.mqtt.iothub.aliyuncs.com\",1883,1\r\n");
    OLED_ShowString(3,5,"85%");
        Delay_ms(5000);
        timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);};
        buf_clear();
        
	//订阅消息
	Serial_Printf("AT+MQTTSUB=0,\"/k0t4jeVASxd/esp_01s_bishe1/user/get\",1\r\n");
    OLED_ShowString(3,5,"95%");
		Delay_ms(1000);
        timeout = 200;
        if(!(strstr((const char*)Serial_RxPacket,"OK"))) while(timeout--){Delay_ms(10);};
        buf_clear();
    OLED_ShowString(3,5,"100%");
    OLED_ShowString(4,6,"OK!");
//printf("AT+MQTTPUB=0,\"/a1A4PwdrMWk/ESP8268/user/update\",\"{\\\"LED\\\":1}\",1,0\r\n");
//  while(1)
//	{
//  if(USART_RX_STA&0x8000) //判断接受是否完成
		
            if(strstr((const char*)Serial_RxPacket,"on")) //输入“on”，LED亮起
            {
                //LED0=0;
            }
            if(strstr((const char*)Serial_RxPacket,"off")) //输入“off”，LED熄灭
            {
                //LED0=1;
            }
            memset(Serial_RxPacket,0,500);
Serial_RxFlag=0;
		}
//	}

 
//上报信息√
uint8_t MQTT_TX_messege(u16 now_temp,u16 now_humi,u16 now_soiltemp,u16 now_soilhumi){
//	float temp;
//	temp = (float)temp/10;
    	

	Serial_Printf("AT+MQTTPUB=0,\"/sys/k0t4jeVASxd/esp_01s_bishe1/thing/event/property/post\",\"{\\\"params\\\":{\\\"temp\\\":%d\\,\\\"humi\\\":%d\\,\\\"soiltemp\\\":%d\\,\\\"soilhumi\\\":%d}}\",1,0\r\n",now_temp,now_humi,now_soiltemp,now_soilhumi);	
    return 1;
}

void MQTT_RX_messege(uint8_t * app_flag)
{
                if(strstr((const char*)Serial_RxPacket,"\"value\":1")) //输入“on”，LED亮起
            {
                *app_flag=1;
            }
            if(strstr((const char*)Serial_RxPacket,"\"value\":0")) //输入“off”，LED熄灭
            {
                *app_flag=0;
            }
            memset(Serial_RxPacket,0,500);
Serial_RxFlag=0;

}



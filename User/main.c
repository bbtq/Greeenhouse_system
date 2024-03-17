#include "stm32f10x.h"                  // Device header
#include "DHT11.h"
#include "OLED.h"
#include "esp8266.h"
#include "usart.h"
#include "DS18B20.h"
#include "AD.h"
#include "LED.h"

char dis0[16];			  
char dis1[16];			


uint16_t ADValue;
uint8_t dick;
uint8_t dick_handle;
uint8_t dick_res;
uint8_t app_flag=3;
 
 
 
int main()
{
 
	u8 tem1=0;
    u8 temlast=0;
	u8 hum1=0;
	
    LED_Init();
    OLED_Init();
    AD_Init();
    
	Serial_Init(115200);  //串口初始化
	OLED_ShowString(1,2,"starting...");
    ESP8266_Init();		  //ESP初始化
    Init_DS18B20();
	
 
	Delay_ms(500);
	 if(!DHT11_Init()){
		 OLED_ShowString(1,4,"Error!DHT11");
	 }
	 Delay_ms(10);		
    OLED_Clear();
	while(1)
	{
		
 
		//DHT11--------------start		
		 if(DHT11_Read_Data(&tem1,&hum1))     
		 {
			OLED_ShowString(1,4,"temp:   "); 
            my_OLED_ShowChar(1,12,95);
            OLED_ShowNum(1,9,tem1,3);
             if(temlast != tem1){
                if(tem1 <= tem_max) GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
                 else GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
             }
             if(hum1>humi_max)GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_SET);
                 else GPIO_WriteBit(GPIOA, GPIO_Pin_7, Bit_RESET);
         if(app_flag==1) GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
         else if(app_flag==0) GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
         else   ;
            OLED_ShowString(2,4,"humi:   %");
            OLED_ShowNum(2,9,hum1,3);
		 }
         

         OLED_ShowString(3,1,"SoilHumi:   %");
         ADValue = get_soilhumi();
         ADValue = ADValue>= 100 ? 100 : ADValue;
         OLED_ShowNum(3,10,ADValue,3);
         delay_ms(10);
         ReadSoilTemperature();        
		
		Delay_ms(250);
        GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		Delay_ms(250);
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
         
        if(app_flag!=3)
        {
           dick_res++;
            if(dick_res==20){
               dick_res=0;
                app_flag=3;
            }
        }
        
        if(Serial_RxFlag == 1)
        {
                 MQTT_RX_messege(&app_flag);
        }
         //10s上传一次数据
         dick = dick%20==0 ? MQTT_TX_messege(tem1,hum1,sdata/100,ADValue) : dick+1;
     }
 }   

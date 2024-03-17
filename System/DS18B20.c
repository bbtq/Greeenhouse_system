#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "DS18B20.h"
#include "OLED.h"

unsigned char tempL=0;         //��ȫ�ֱ���
unsigned char tempH=0; 
unsigned int sdata;            //�¶ȵĲ���
unsigned char fg=1;                    //�¶�������־
unsigned int last_sdata = 1500;



void DS_DIR_IN(void)  //��PB9Ϊ��������ģʽ
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}
 
void DS_DIR_OUT(void) //��PB9Ϊ�������ģʽ
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

void Init_DS18B20(void) 
{
	unsigned char t = 0;
	DS_DIR_OUT();  //��GPIO��Ϊ�������ģʽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);  //���͸�λ���� ds18b20 DQ�ܽŽӵ���Ƭ����PB9
	Delay_us(600); 		//��ʱ��>480us) 
	GPIO_SetBits(GPIOB,GPIO_Pin_9);		//���������� 
	Delay_us(20); 				//�ȴ���15~60us)	
	DS_DIR_IN(); //����GPIO��Ϊ��������ģʽ
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == SET) //�ȴ�����
	{
		Delay_us(1);
		t++;
		if(t >= 240)//�������240us���Ǹߵ�ƽ ����ds18b20û�����ݻ��� ��ʧ������
        {
            OLED_ShowString(2,3,"No Down");
            return;   
        }
    }
	t = 0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == RESET) //�ȴ�����
	{
		Delay_us(1);
		t++;
		if(t >= 240)//�������240us���ǵ͵�ƽ ����ds18b20û�����ݻ��� ��ʧ������
        {
            OLED_ShowString(2,3,"No Up");
            return;   
        }
	}
    OLED_ShowString(4,1,"SoilTemp  OK!");
	//printf("�¶ȳ�ʼ����ɣ�"); //�ҵ�printf�����Ǹ����ڽ���ˣ����ڿ�ds18b20�Ƿ��ʼ�����
}

//��Ƭ����DS18B20дһλ  0  ��Ӧ��ͼ���� ���Ϸ���ͼ
void Write_Bit_0(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_9); //����
	delay_us(90);  //��60us --- 120us֮��
	GPIO_SetBits(GPIOB,GPIO_Pin_9);  //����
	delay_us(10);  //���ߴ���1us ����ѡ��10us
}
//��Ƭ����DS18B20дһλ 1
void Write_Bit_1(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_9); //���� 
	delay_us(10);  //���ʹ���1us
	GPIO_SetBits(GPIOB,GPIO_Pin_9);  //����
	delay_us(90); //�ҵ������ ���ߴ���60us����д1 ����ѡ��90us
}
 
//��Ƭ����DS18B20��һλ 
unsigned char Read_Bit(void)
{
	DS_DIR_OUT(); //PB9Ϊ�������ģʽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);  //����
	delay_us(10); //����1us ѡ��10us
	DS_DIR_IN(); //PB9Ϊ��������ģʽ
	delay_us(10); //�ȴ�һ�� ���ں����жϸùܽŷ��ص��Ǹߵ�ƽ���ǵ͵�ƽ ̫�󲻺��Թ�80us���ص����ݾ������
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == SET)
	{
		return 1; //�ߵ�ƽ����1 
	}
	else
	{
		return 0; //�͵�ƽ����0
	}
}

//��һ���ֽ�
unsigned char ReadOneChar(void)  			
{
	unsigned char i=0; 		
	unsigned char dat=0; 
	for (i=0;i<8;i++) 		
	{
		dat = dat | (Read_Bit() << i); //DS18B20���ֲ����ǵ�λ��ʼ��������ģ�Ҫ��ȡһ���������ֽڣ������ӾͿ��ԡ�
	} 
	return(dat);
}
 
//дһ���ֽ�
void WriteOneChar(unsigned char dat) //datҪ���͵�����
{ 
	unsigned char i=0; 		
	DS_DIR_OUT(); //�������
	for(i=8;i>0;i--) 		//��15~60us֮��������߽��в���������Ǹߵ�ƽ��д1����д0������ 
	{
		if((dat & 0x01) == 1)
		{
			Write_Bit_1();
		}
		else
		{
			Write_Bit_0();
		}
		dat >>= 1;
	} 
}

//���¶�ֵ����λ��tempL;��λ��tempH;��
void ReadSoilTemperature(void) 
{ 
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ���
	WriteOneChar(0x44); 				//�����¶�ת��
	delay_us(1000);					    //ת����Ҫһ��ʱ�䣬��ʱ 
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ��� 
	WriteOneChar(0xbe); 				//���¶ȼĴ�����ͷ����ֵ�ֱ�Ϊ�¶ȵĵ�λ�͸�λ��	
	tempL=ReadOneChar(); 				//�����¶ȵĵ�λLSB
	tempH=ReadOneChar(); 				//�����¶ȵĸ�λMSB	
	if(tempH>0x7f)      				//���λΪ1ʱ�¶��Ǹ�
	{
		tempL=~tempL;					//����ת����ȡ����һ
		tempH=~tempH+1;       
		fg=0;      						//��ȡ�¶�Ϊ��ʱfg=0
	}
	sdata = (tempH << 8) + tempL;
	sdata = (sdata * 0.0625) * 100;  //�����100 ���ڱ�����λС���ˣ���Ϊ����unsigned int���Ͳ���float��
    sdata = abs(sdata - last_sdata) >1000 ? last_sdata : sdata;
    last_sdata = sdata;
    OLED_ShowString(4,9,":");
    OLED_ShowNum(4,10,(sdata/100),2);
    OLED_ShowString(4,12,".");
    OLED_ShowNum(4,13,(sdata%100),2);
    my_OLED_ShowChar(4,15,95);
//	printf("%d  \r\n",sdata);
}

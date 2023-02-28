/***************************************************
����TI��˾��MSP430F5529оƬ
ʹ��MSP-EXP430F5529LP������
ʵ�ֵĹ���Ϊ��
1������MSP430�ڲ�PWM������������Ƶ��Ϊ 50~1kHz��ռ�ձ�Ϊ10%~90%��PWM���� 
2��Ҫ��Ƶ�ʺ�ռ�ձ�ͨ������S1��S2�ֱ����á�Ƶ �ʰ�50Hz������ռ�ձȰ�10%������
3������MSP430��Ƭ���Ĳ���/�Ƚ���������PWM�� ��Ƶ�ʺ�ռ�ձȣ�����ʾ��oled��Ļ�ϡ�

������ʹ�õ���ʾ���������oled12864��
���ӷ�ʽΪSDA������P6.5
          SCL������P6.0
          ͬʱʹ�õ�VCC��3V
 
 P1.2�ں�P1.4���������Ŀɵ�Ƶ�ʺ����ڵ�PWM��
 P2.0Ϊ�������룬������Ҫ����P1.2�ں�P2.0��
***************************************************/

#include "MSP430F5529.h"
#include "clk.h"
#include "key.h"
#include "timer.h"
#include "IIC.h"

/****************��Ҫ��ʾ��һϵ���ַ���***************/
  unsigned char string1[]={"PWM&Capture"};
  unsigned char string2[]={"Frequency:"};
  unsigned char string3[]={"post_duty:"};
  unsigned char string4[]={"Hz"};
  unsigned char string5[]={"%"};
  
void  main( )
{
  enum key_type key;
  unsigned int duty_num=10; //���ڿ���ռ�ձȵı���
  unsigned int fre_num=100; //���ڿ���Ƶ�ʵı���              
  float  duty=0;           //���ڼ���ռ�ձ�
  float  fre=0;            //���ڼ���Ƶ��
  
  WDTCTL = WDTPW+WDTHOLD;  // �رտ��Ź�
  ClkInit();               //ʱ�ӳ�ʼ��
  KeyInit();               //����������ʼ��
  LCD_Init();              //��ʾ����ʼ��
  PWM_Init();              //PWM�������ʼ��
  Capture_Init();          //���벶���ʼ��
  
  /*��ʼ������*/
  //LCD_ShowString(0,0,string1,16);
 // LCD_ShowString(0,0,string2,12);
 // LCD_ShowString(0,2,string3,12);
  //LCD_ShowString(112,0,string4,12);
  //LCD_ShowString(112,2,string5,12);
  LCD_ShowString(2,3,string2,12);
  LCD_ShowString(2,4,string3,12);
  LCD_ShowString(110,3,string4,12);
  LCD_ShowString(110,4,string5,12);
  while(1)
  {
    key = KeyScan();
    if( key != NOKEY )
    {
      switch(key)
      {
        case KEYLEFT : 
          {
            duty_num+=10;
            
            if(duty_num>90) //��ֹռ�ձȳ�����Χ
            {
            duty_num=10;
            }
            break;
          }      
        case KEYRIGHT :   
          {
            fre_num+=100;
            if(fre_num>2000) //��ֹƵ�ʳ�����Χ
            {
              fre_num=100;
            }
            
            break;
          }
      }
      TA0CCR0=1000000/fre_num ;
      TA0CCR1=(int)(TA0CCR0*(((float)duty_num)/100));
      TA0CCR2=TA0CCR0;
      TA0CCR3=TA0CCR1;
    }
  
    if(flag==3)
    {
     duty = 100*(((float)pos_time)/((float)period))+1; //����ռ�ձ�
     
     fre=500000/(float)period+1;  //����Ƶ��
     
     LCD_ShowNum(87,3,(unsigned long)fre,3,12); //��ʾռ�ձȺ�Ƶ��  
     //LCD_ShowNum(87,5,(unsigned long)duty,3,12); 
     LCD_ShowNum(87,4,duty_num,3,12); 
      
      flag=0;
      TA1CCTL1|=CCIE;
    }  
  }
}
  
  
 
  
  



/***************************************************
采用TI公司的MSP430F5529芯片
使用MSP-EXP430F5529LP开发板
实现的功能为：
1、利用MSP430内部PWM波发生器产生频率为 50~1kHz，占空比为10%~90%的PWM波。 
2、要求频率和占空比通过按键S1和S2分别设置。频 率按50Hz步进，占空比按10%步进。
3、利用MSP430单片机的捕获/比较器测量该PWM波 的频率和占空比，且显示于oled屏幕上。

（这里使用的显示屏是四针的oled12864）
连接方式为SDA口连接P6.5
          SCL口连接P6.0
          同时使用的VCC是3V
 
 P1.2口和P1.4口输出反相的可调频率和周期的PWM波
 P2.0为捕获输入，所以需要连接P1.2口和P2.0口
***************************************************/

#include "MSP430F5529.h"
#include "clk.h"
#include "key.h"
#include "timer.h"
#include "IIC.h"

/****************需要显示的一系列字符串***************/
  unsigned char string1[]={"PWM&Capture"};
  unsigned char string2[]={"Frequency:"};
  unsigned char string3[]={"post_duty:"};
  unsigned char string4[]={"Hz"};
  unsigned char string5[]={"%"};
  
void  main( )
{
  enum key_type key;
  unsigned int duty_num=10; //用于控制占空比的变量
  unsigned int fre_num=100; //用于控制频率的变量              
  float  duty=0;           //用于计算占空比
  float  fre=0;            //用于计算频率
  
  WDTCTL = WDTPW+WDTHOLD;  // 关闭看门狗
  ClkInit();               //时钟初始化
  KeyInit();               //独立按键初始化
  LCD_Init();              //显示屏初始化
  PWM_Init();              //PWM波输出初始化
  Capture_Init();          //输入捕获初始化
  
  /*初始化界面*/
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
            
            if(duty_num>90) //防止占空比超出范围
            {
            duty_num=10;
            }
            break;
          }      
        case KEYRIGHT :   
          {
            fre_num+=100;
            if(fre_num>2000) //防止频率超出范围
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
     duty = 100*(((float)pos_time)/((float)period))+1; //计算占空比
     
     fre=500000/(float)period+1;  //计算频率
     
     LCD_ShowNum(87,3,(unsigned long)fre,3,12); //显示占空比和频率  
     //LCD_ShowNum(87,5,(unsigned long)duty,3,12); 
     LCD_ShowNum(87,4,duty_num,3,12); 
      
      flag=0;
      TA1CCTL1|=CCIE;
    }  
  }
}
  
  
 
  
  



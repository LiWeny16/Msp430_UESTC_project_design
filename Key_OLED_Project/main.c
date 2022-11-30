#include "MSP430f5529.h"
#include "IIC.c"
int m,n,k;
long int a,a1,a2,a3,a4,a5;
void delay(k)
{
  for(m=0;m<k;m++)
    for(n=0;n<100;n++);
}//延时函数
void show(long int a)
{
  if(a>=10000||a<=-10000)
  {
    LCD_ShowNum(35,3,a,5,16);
  }
  else if(a>=1000||a<=-1000)
  {
    LCD_ShowNum(35,3,a,4,16);
  }
  else if(a>=100||a<=-100)
  {
    LCD_ShowNum(35,3,a,3,16);
  }
  else if(a>=10||a<=-10)
  {
    LCD_ShowNum(35,3,a,2,16);
  }
  else
  {
    LCD_ShowNum(35,3,a,1,16);
  }
}//显示数字的函数
void main( void )
{
  
  WDTCTL = WDTPW + WDTHOLD;// 踢狗
  P1DIR = BIT0;  //设置P1.0输出
  P4DIR = BIT7;  //设置P4.7输出
  P2REN = BIT1;//设置上拉、下拉电阻允许
  P1REN = BIT1;
  P2OUT = BIT1;//设置上拉电阻
  P1OUT = BIT1;
  P2IE = BIT1;//中断使能
  P2IES = BIT1;//下降沿中断
  P1IE = BIT1;
  P1IES = BIT1;
  P2IFG = 0;//标志位清零
  P1IFG = 0;
  _EINT();//开总中断
  a = 0;//给a一个初值
  LCD_Init();//LCD初始化
  LCD_ShowPicture(30,1);//显示校徽
  delay(1000);//延时
  LCD_Init();//初始化
  LCD_ShowString(35,3,"UESTC",16);//显示UESTC
  delay(1000);//延时
  LCD_Init();//初始化
  while(1)
  {
    if(a>65535)
      a=65535;
    else if(a<-65535)
      a=-65535;//控制显示范围 2的16次方
    if(a%2 == 0)
    {
      P4OUT = BIT7;
      P1OUT = BIT1;
    }
    else
    {
      P1OUT = 0x03;
      P4OUT&=~BIT7; 
    }//判断奇数还是偶数，并使相应的小灯亮
    if(a<0)
    {
      LCD_ShowChar(25,3,'-',16);
      show(-a);
    }
    else
    {
      LCD_ShowChar(25,3,'+',16);
      show(a);
    }//扩展显示范围到负数
  }
}
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)//2.1中断服务函数
{
  if(P2IFG&BIT1)
  {
    delay(10);//延时消抖
    if(P2IN==0xFD)
       a++;
   }
  P2IFG = 0;//标志位清零
}
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)//1.1中断服务函数
{
  if(P1IFG&BIT1)
  {
    delay(10);//延时消抖
    if(P1IN==0xFD||P1IN==0xFC)
      a--;
  }
  P1IFG = 0;//标志位清零
}


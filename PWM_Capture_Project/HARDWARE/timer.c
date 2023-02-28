/************************************************************
  定时器的设置
  文件名称：                timer.c
  文件概述：                对TA0输出PWM波 TA1进行输入捕获的配置工作
  包含的头文件及概述：      timer.h ：函数声明及部分宏定义                         
  引脚声明：                
                            TA0 PWM波输出口   ：P1.2
                            TA1 输入捕获输入口：P2.0
*************************************************************/ 


#include 	"msp430f5529.h"
#include  	"timer.h"

char flag=0;                       //用于标记当前捕获状态 0:就绪 1：捕获到一个上升沿 2：捕获到一个下降沿 
unsigned int pos_time=0;          //用于记录正脉宽
unsigned int period  =0;          //用于记录周期


unsigned int start   =0;          //开始测量上升沿
unsigned int pause   =0;
unsigned int num[3];
unsigned int T,TP,D;
/******************

PWM波初始化
输出两种反相的PWM波

********************/
void PWM_Init( void )
{
        
	P1DIR |= BIT2; //配置P1.2复用为定时器TA0.1
        P1SEL |= BIT2; //配置P1.2为输出
        P1DIR |= BIT4; //配置P1.4复用为定时器TA0.3
        P1SEL |= BIT4; //配置P1.4为输出
        TA0CTL = TASSEL_2 + MC_1 + TACLR + ID_3;//使用SMCLK为时钟源 UP计数模式 8分频
        TA0CCTL1 = OUTMOD_7 ; //TA0输出模式7
        TA0CCTL3 = OUTMOD_3 ; //TA3输出模式3
        TA0CCR0 = 10000 ;   //初始化频率50赫兹
        TA0CCR1 = 1000 ;    //初始化占空比为10%
        
}

/******************
捕捉器初始化
Capture 函数

********************/

void Capture_Init(void)
{
          P2DIR &= ~BIT0 ; //配置P2.0复用为定时器TA1.1
          P2SEL |= BIT0 ; //配置P2.0为输入
   
          TA1CTL = TASSEL_2 + MC_2 + TACLR + ID_3 ; //使用SMCLK为时钟源 连续计数模式 8分频
          TA1CCTL1 = CM0 + SCS + CAP + CCIE +CCIS_0; //捕获模式 上升沿捕获 同步 中断使能 CCI1A(P 2.0)
        _BIS_SR( GIE );		                //使能系统总中断
}

/*中断子函数*/

#pragma	vector = TIMER1_A1_VECTOR
__interrupt void Timer_A1 ( void )
{
  if(TA1IV==2)
  {
    if(P2IN&BIT0)
    {
      if(flag==0)
      {
        start = TA1CCR1;
        flag = 1;
      }
      if(flag==2)
      {
        period = TA1CCR1-start;
        flag=3;
        TA1CCR1 = 0;
        TA1CCTL1&=~CCIE;
      }
      else
      {
        if(flag==1)
        {
          pos_time = TA1CCR1-start;
          flag = 2;
        }
      }
      
    }
  }
  
}







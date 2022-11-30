#include "msp430f5529.h"
#include "IIC.c"
#include "stdio.h"

unsigned int HZ[4]={25,50,75,100};
unsigned int index=0;
unsigned int test_hz;
unsigned int flag;
float result;
int a,b;

 void delay(int t)   //延时t ms
 {
    
    while(t--)
    {
     int m=200;
     while(m--);
    }
 }
int main( void )
{
  void delay();
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW+WDTHOLD;
  
  P5SEL |= BIT4|BIT5; //配置为XT1功能,电路板上晶振接于这两脚
  UCSCTL6 &= ~(XT1OFF);// XT1打开
  while (UCSCTL7&XT1LFOFFG)
    UCSCTL7 &= ~XT1LFOFFG;//清除错误标志位
  //ACLK默认以XT1为时钟源
  UCSCTL4 = SELA__XT1CLK+SELM__DCOCLK+SELS__DCOCLK;

  P2DIR|=BIT0;  //p2.0为方波输出端口
  
  TA0CCR0=(int)(32768/(2*HZ[index]));  //设置半周期中断一次，使输出反转 
  TA0CTL|=TASSEL_1+MC_1+TACLR;   //TA0选择ACLK为信号源（32.768KHz），增计数模式，定时器清零
  TA0CCTL0=CCIE;    //使能中断
  
  P6SEL|=BIT1;
  P6DIR&=~BIT1; //ADC12选择6.1端口通道（A1通道）
  
  ADC12CTL0=ADC12SHT0_2+ADC12ON+ADC12REFON+ADC12REF2_5V;   //设置采样时间为16个时钟周期（ADC12CLK,5MHz）+启动adc12
  ADC12CTL1=ADC12SHP+ADC12CSTARTADD_1+ADC12CONSEQ_0; 
  //脉冲采样模式（外部时钟信号上升沿触发采样，采样定时器产生采样信号）+存放地址选择MEM1+单通道单次采样
  ADC12MCTL1|=ADC12INCH_1; //选择通道A1
   

  P2DIR&=~BIT1;
  P2IFG=0x00;  
  P2IE|=BIT1;
  P2IES|=BIT1;
  P2REN|=BIT1;
  P2OUT|=BIT1;  //按键中断设置
  
  LCD_Init(); 
  _EINT(); //使能中断
  while(1)
  {
    ADC12CTL0|=ADC12ENC;
    ADC12CTL0|=ADC12SC;  //开始采样
 
    TA1CTL|=TASSEL_1+MC_1+TACLR; //设置定时器A1
    TA1CCR0=0XFFFF;
    while(TA1R<32768)  //定时1S
    {
      if((P2OUT&BIT0)==0)
        flag=1;
      if((flag==1)&&(P2OUT&BIT0))
      {
        test_hz++; //检测到上升沿，加一，即频率
        flag=0;
      }
    }
    LCD_ShowNum(10,1,test_hz,4,16); //显示频率
    LCD_ShowChar(50,1,'H',16);
    LCD_ShowChar(58,1,'z',16);
    test_hz=0;  
    if(ADC12IFG1)//检测转换完成
    {
      result=ADC12MEM1;//读取采样的值
    }
    /*if(!result)
    {
       float v_result=(2.5*result)/4096;
      LCD_ShowNum(10,6,v_result,4,16);
      LCD_ShowChar(42,6,'m',16);
      LCD_ShowChar(50,6,'v',16);
      
    }*/
    if(result){
       float  v_result=(3.3*result)/4096;
      LCD_ShowNum(10,6,v_result*1000,4,16);
    LCD_ShowChar(42,6,'m',16);
      LCD_ShowChar(50,6,'v',16);
    }
  }
}
//TIMEA0 中断
#pragma vector=TIMER0_A0_VECTOR
__interrupt void time_a0(void)
{
  P2OUT^=0x01;    //输出电平反转
  TA0CTL ^= TAIFG;//TAIFG清除
}

//按键中断
#pragma vector=PORT2_VECTOR
__interrupt void Port2_1()
{
  
  if(P2IFG&BIT1)
  {
    
    
    /*TA1CCR0=0xffff;
    TA1CTL|=TASSEL_1+MC_1+TACLR;
    while(TA1R<=655);    //利用timer1 延时20ms
    if((P2IN&BIT1)==0)
    {
      index++;
      if(index==4)
        index=0;
    }  
  }*/
    if(P2IN==0xFD){
      delay(300); //按键消抖
      if(index==4)
        index=0;
    }
    else {
      index++;
    }
    TA0CCR0=(int)(32768/(2*HZ[index])); //修改方波频率
    }
  P2IFG&=~BIT1;//清除标志位
  }


#include "msp430f5529.h"
#include "IIC.c"
#include "stdio.h"

unsigned int HZ[4]={25,50,75,100};
unsigned int index=0;
unsigned int test_hz;
unsigned int flag;
float result;
int a,b;

 void delay(int t)   //��ʱt ms
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
  
  P5SEL |= BIT4|BIT5; //����ΪXT1����,��·���Ͼ������������
  UCSCTL6 &= ~(XT1OFF);// XT1��
  while (UCSCTL7&XT1LFOFFG)
    UCSCTL7 &= ~XT1LFOFFG;//��������־λ
  //ACLKĬ����XT1Ϊʱ��Դ
  UCSCTL4 = SELA__XT1CLK+SELM__DCOCLK+SELS__DCOCLK;

  P2DIR|=BIT0;  //p2.0Ϊ��������˿�
  
  TA0CCR0=(int)(32768/(2*HZ[index]));  //���ð������ж�һ�Σ�ʹ�����ת 
  TA0CTL|=TASSEL_1+MC_1+TACLR;   //TA0ѡ��ACLKΪ�ź�Դ��32.768KHz����������ģʽ����ʱ������
  TA0CCTL0=CCIE;    //ʹ���ж�
  
  P6SEL|=BIT1;
  P6DIR&=~BIT1; //ADC12ѡ��6.1�˿�ͨ����A1ͨ����
  
  ADC12CTL0=ADC12SHT0_2+ADC12ON+ADC12REFON+ADC12REF2_5V;   //���ò���ʱ��Ϊ16��ʱ�����ڣ�ADC12CLK,5MHz��+����adc12
  ADC12CTL1=ADC12SHP+ADC12CSTARTADD_1+ADC12CONSEQ_0; 
  //�������ģʽ���ⲿʱ���ź������ش���������������ʱ�����������źţ�+��ŵ�ַѡ��MEM1+��ͨ�����β���
  ADC12MCTL1|=ADC12INCH_1; //ѡ��ͨ��A1
   

  P2DIR&=~BIT1;
  P2IFG=0x00;  
  P2IE|=BIT1;
  P2IES|=BIT1;
  P2REN|=BIT1;
  P2OUT|=BIT1;  //�����ж�����
  
  LCD_Init(); 
  _EINT(); //ʹ���ж�
  while(1)
  {
    ADC12CTL0|=ADC12ENC;
    ADC12CTL0|=ADC12SC;  //��ʼ����
 
    TA1CTL|=TASSEL_1+MC_1+TACLR; //���ö�ʱ��A1
    TA1CCR0=0XFFFF;
    while(TA1R<32768)  //��ʱ1S
    {
      if((P2OUT&BIT0)==0)
        flag=1;
      if((flag==1)&&(P2OUT&BIT0))
      {
        test_hz++; //��⵽�����أ���һ����Ƶ��
        flag=0;
      }
    }
    LCD_ShowNum(10,1,test_hz,4,16); //��ʾƵ��
    LCD_ShowChar(50,1,'H',16);
    LCD_ShowChar(58,1,'z',16);
    test_hz=0;  
    if(ADC12IFG1)//���ת�����
    {
      result=ADC12MEM1;//��ȡ������ֵ
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
//TIMEA0 �ж�
#pragma vector=TIMER0_A0_VECTOR
__interrupt void time_a0(void)
{
  P2OUT^=0x01;    //�����ƽ��ת
  TA0CTL ^= TAIFG;//TAIFG���
}

//�����ж�
#pragma vector=PORT2_VECTOR
__interrupt void Port2_1()
{
  
  if(P2IFG&BIT1)
  {
    
    
    /*TA1CCR0=0xffff;
    TA1CTL|=TASSEL_1+MC_1+TACLR;
    while(TA1R<=655);    //����timer1 ��ʱ20ms
    if((P2IN&BIT1)==0)
    {
      index++;
      if(index==4)
        index=0;
    }  
  }*/
    if(P2IN==0xFD){
      delay(300); //��������
      if(index==4)
        index=0;
    }
    else {
      index++;
    }
    TA0CCR0=(int)(32768/(2*HZ[index])); //�޸ķ���Ƶ��
    }
  P2IFG&=~BIT1;//�����־λ
  }


#include "MSP430f5529.h"
#include "IIC.c"
int m,n,k;
long int a,a1,a2,a3,a4,a5;
void delay(k)
{
  for(m=0;m<k;m++)
    for(n=0;n<100;n++);
}//��ʱ����
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
}//��ʾ���ֵĺ���
void main( void )
{
  
  WDTCTL = WDTPW + WDTHOLD;// �߹�
  P1DIR = BIT0;  //����P1.0���
  P4DIR = BIT7;  //����P4.7���
  P2REN = BIT1;//����������������������
  P1REN = BIT1;
  P2OUT = BIT1;//������������
  P1OUT = BIT1;
  P2IE = BIT1;//�ж�ʹ��
  P2IES = BIT1;//�½����ж�
  P1IE = BIT1;
  P1IES = BIT1;
  P2IFG = 0;//��־λ����
  P1IFG = 0;
  _EINT();//�����ж�
  a = 0;//��aһ����ֵ
  LCD_Init();//LCD��ʼ��
  LCD_ShowPicture(30,1);//��ʾУ��
  delay(1000);//��ʱ
  LCD_Init();//��ʼ��
  LCD_ShowString(35,3,"UESTC",16);//��ʾUESTC
  delay(1000);//��ʱ
  LCD_Init();//��ʼ��
  while(1)
  {
    if(a>65535)
      a=65535;
    else if(a<-65535)
      a=-65535;//������ʾ��Χ 2��16�η�
    if(a%2 == 0)
    {
      P4OUT = BIT7;
      P1OUT = BIT1;
    }
    else
    {
      P1OUT = 0x03;
      P4OUT&=~BIT7; 
    }//�ж���������ż������ʹ��Ӧ��С����
    if(a<0)
    {
      LCD_ShowChar(25,3,'-',16);
      show(-a);
    }
    else
    {
      LCD_ShowChar(25,3,'+',16);
      show(a);
    }//��չ��ʾ��Χ������
  }
}
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)//2.1�жϷ�����
{
  if(P2IFG&BIT1)
  {
    delay(10);//��ʱ����
    if(P2IN==0xFD)
       a++;
   }
  P2IFG = 0;//��־λ����
}
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)//1.1�жϷ�����
{
  if(P1IFG&BIT1)
  {
    delay(10);//��ʱ����
    if(P1IN==0xFD||P1IN==0xFC)
      a--;
  }
  P1IFG = 0;//��־λ����
}


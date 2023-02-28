/************************************************************
  ��ʱ��������
  �ļ����ƣ�                timer.c
  �ļ�������                ��TA0���PWM�� TA1�������벶������ù���
  ������ͷ�ļ���������      timer.h ���������������ֺ궨��                         
  ����������                
                            TA0 PWM�������   ��P1.2
                            TA1 ���벶������ڣ�P2.0
*************************************************************/ 


#include 	"msp430f5529.h"
#include  	"timer.h"

char flag=0;                       //���ڱ�ǵ�ǰ����״̬ 0:���� 1������һ�������� 2������һ���½��� 
unsigned int pos_time=0;          //���ڼ�¼������
unsigned int period  =0;          //���ڼ�¼����


unsigned int start   =0;          //��ʼ����������
unsigned int pause   =0;
unsigned int num[3];
unsigned int T,TP,D;
/******************

PWM����ʼ��
������ַ����PWM��

********************/
void PWM_Init( void )
{
        
	P1DIR |= BIT2; //����P1.2����Ϊ��ʱ��TA0.1
        P1SEL |= BIT2; //����P1.2Ϊ���
        P1DIR |= BIT4; //����P1.4����Ϊ��ʱ��TA0.3
        P1SEL |= BIT4; //����P1.4Ϊ���
        TA0CTL = TASSEL_2 + MC_1 + TACLR + ID_3;//ʹ��SMCLKΪʱ��Դ UP����ģʽ 8��Ƶ
        TA0CCTL1 = OUTMOD_7 ; //TA0���ģʽ7
        TA0CCTL3 = OUTMOD_3 ; //TA3���ģʽ3
        TA0CCR0 = 10000 ;   //��ʼ��Ƶ��50����
        TA0CCR1 = 1000 ;    //��ʼ��ռ�ձ�Ϊ10%
        
}

/******************
��׽����ʼ��
Capture ����

********************/

void Capture_Init(void)
{
          P2DIR &= ~BIT0 ; //����P2.0����Ϊ��ʱ��TA1.1
          P2SEL |= BIT0 ; //����P2.0Ϊ����
   
          TA1CTL = TASSEL_2 + MC_2 + TACLR + ID_3 ; //ʹ��SMCLKΪʱ��Դ ��������ģʽ 8��Ƶ
          TA1CCTL1 = CM0 + SCS + CAP + CCIE +CCIS_0; //����ģʽ �����ز��� ͬ�� �ж�ʹ�� CCI1A(P 2.0)
        _BIS_SR( GIE );		                //ʹ��ϵͳ���ж�
}

/*�ж��Ӻ���*/

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







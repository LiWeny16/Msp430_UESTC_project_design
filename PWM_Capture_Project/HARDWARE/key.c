/********************************************************************************* 
  *Copyright(C),UESTC
  *FileName:    独立按键配置
  *Author:      Tonge
  *Version:     1.0 
  *Date:        2016.12.2
  *Description:  配置msp430f5529的独立按键

                  MSP430F5529
              -----------------
         /|\|                 |
         |  |                 |
         ---|RST              |
            |             P2.1|-->KEYLEFT
            |             P1.1|-->KEYRIGHT
  *Others:      None
  *Function List:  
         1.void KeyInit(void) 独立按键配置初始化，上电调用一次即可。
         2.enum key_type KeyScan(void)按键扫描程序返回枚举型键值
                  其中key_type{NOKEY,KEYLEFT,KEYRIGHT};
**********************************************************************************/  
#include 	"msp430f5529.h"
#include  	"key.h"

static	enum    key_type key_value = NOKEY;
static  int 	key_down_flag = 0;
enum key_type	KeyScan(void)
{
        
	if( KEY_L )	
            key_value = KEYLEFT;
        
	else if( KEY_R )		
            key_value = KEYRIGHT;
        
        else
            key_value = NOKEY;
        
        //防止一次按下多次响应，不等待按键抬起，不过多占用CPU
        if      (key_down_flag == 0 && key_value != NOKEY)
        {
            key_down_flag = 1;//第一次监测到按键，按下标志置位，返回此时键值

        }
        else if (key_down_flag == 1 && key_value != NOKEY)
        {
            key_value     = NOKEY;//第一次之后检测按键按下，返回空值
        }
        else
        {
            key_down_flag = 0;//检测按键抬起，按下标识复位，为下一次按下准备
        }
	                        
	return	key_value;
}
void KeyInit(void)
{
	P1DIR	&= ~BIT1;       //设置按键IO为输入模式
	P2DIR	&= ~BIT1;
        P1REN   |= BIT1;        //使能按键上、下拉电阻
        P2REN   |= BIT1;
        P1OUT   |= BIT1;        //设置按键IO为上拉电阻模式
        P2OUT   |= BIT1;
}
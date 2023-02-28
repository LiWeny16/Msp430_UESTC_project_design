#include        "IIC.h"
#include        "font.h"

/************************************************************
  文件名称：                IIC.c
  文件概述：                通过配置IIC通信接口对oled进行控制
  包含的头文件及概述：      IIC.h ：函数声明及部分宏定义
                            font.h：常用字库 额外字符可自行添加
  引脚声明：                
                            IIC通信时钟线：P6.0
                            IIC通信数据线：P6.5
*************************************************************/  



/************************************************************/
//函数名     ：IIC_delayus
//函数输入值 ：i：延时控制变量
//函数返回值 ：无
//函数功能   ：延时1us
//注意事项   ：这是在4M时钟的前提下进行的分频操作，倘若机器时钟有所变化，该函数也要进行相应的变化
void IIC_delayus(unsigned int i)
{
  unsigned int j,k;
  for(k=0;k<i;k++)
    for(j=0;j<4;i++);
}
/************************************************************/



/************************************************************/
//函数名     ：IIC_ioinit
//函数输入值 ：无
//函数返回值 ：无
//函数功能   ：对IIC通信使用的I/O口进行配置
void IIC_ioinit()
{
  P6DIR|=BIT0|BIT5; //配置IIC IO口为输出方向
  P6REN|=BIT0|BIT5; //配置IIC IO口使能上拉电阻
}
/************************************************************/




/************************************************************/
//函数名     ：IIC_start
//函数输入值 ：无
//函数返回值 ：无
//函数功能   ：开始IIC通信的操作
void IIC_start()
{
   SCL_HIGH;
   SDA_HIGH;
   //IIC_delayus(2); //*此处根据情况是否添加延时
   SDA_LOW;
   //IIC_delayus(2);
   SCL_LOW;
}
/************************************************************/




/************************************************************/
//函数名     ：IIC_stop
//函数输入值 ：无
//函数返回值 ：无
//函数功能   ：停止IIC通信的操作
void IIC_stop()
{
     SCL_LOW;
     SDA_LOW;
     //IIC_delayus(2);
     SCL_HIGH;
     SDA_HIGH;
     //IIC_delayus(2);
}
/************************************************************/



/************************************************************/
//函数名     ：IIC_writebyte
//函数输入值 ：IIC_byte：所要发送的字节内容
//函数返回值 ：无
//函数功能   ：向IIC总线发送一个字节
void IIC_writebyte(unsigned char IIC_byte)
{
   unsigned char i;
   for(i=0;i<8;i++)  
   {
      if(IIC_byte&0x80)  
        SDA_HIGH;
      else
        SDA_LOW;
      SCL_HIGH;
      SCL_LOW;
      IIC_byte<<=1;   //loop
   }
   SDA_HIGH;
   SCL_HIGH;
   SCL_LOW;
}
/************************************************************/




/************************************************************/
//函数名     ：IIC_writecmd
//函数输入值 ：IIC_command：所要发送的指令内容
//函数返回值 ：无
//函数功能   ：向IIC总线发送一条指令
void IIC_writecmd(unsigned char IIC_command)
{
   IIC_start();
   IIC_writebyte(0x78);   //Slave address,SA0=0
   IIC_writebyte(0x00);   //write command
   IIC_writebyte(IIC_command);
   IIC_stop();
}
/************************************************************/



/************************************************************/
//函数名     ：IIC_writedata
//函数输入值 ：IIC_data：所要发送的数据内容
//函数返回值 ：无
//函数功能   ：向IIC总线发送一条数据
void IIC_writedata(unsigned char IIC_data)
{
    IIC_start();
    IIC_writebyte(0x78);   
    IIC_writebyte(0x40);   //write data
    IIC_writebyte(IIC_data);
    IIC_stop();
}
/************************************************************/



// 说明：接下来的函数是通过IIC操作对oled进行控制的函数，直接根据功能调用即可
// 函数列表及对应功能：
// 1、LCD_Set_Pos   ：设置oled的坐标
// 2、LCD_CLS       ：对oled进行清屏
// 3、LCD_Init      ：对oled进行初始化操作
// 4、LCD_ShowChar  ：在指定坐标处显示一个字符（函数接口在下方有写）
// 5、LCD_ShowString：在指定坐标处显示一个字符串
// 6、LCD_ShowNum   ：在指定坐标处显示一个数字




/**************************oled设置坐标*******************************/
void LCD_Set_Pos(unsigned char x, unsigned char y)
{
    IIC_writecmd(0xb0+y);
    IIC_writecmd(((x&0xf0)>>4)|0x10);
    IIC_writecmd((x&0x0f)|0x01);
}


/***************************oled清屏************************************/
void LCD_CLS(void)
{
  unsigned char y,x;        
  for(y=0;y<8;y++)
  {
    IIC_writecmd(0xb0+y);
    IIC_writecmd(0x01);
    IIC_writecmd(0x10);
    for(x=0;x<X_WIDTH;x++)
    IIC_writedata(0);
   }
}


/***************************oled初始化************************************/
void LCD_Init(void)
{
  IIC_ioinit();
  
  
  
  IIC_writecmd(0xAE);   //display off
  IIC_writecmd(0x20); //Set Memory Addressing Mode
  IIC_writecmd(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  IIC_writecmd(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
  IIC_writecmd(0xc8); //Set COM Output Scan Direction
  IIC_writecmd(0x00);//---set low column address
  IIC_writecmd(0x10);//---set high column address
  IIC_writecmd(0x40);//--set start line address
  IIC_writecmd(0x81);//--set contrast control register
  IIC_writecmd(0x7f);
  IIC_writecmd(0xa1);//--set segment re-map 0 to 127
  IIC_writecmd(0xa6);//--set normal display
  IIC_writecmd(0xa8);//--set multiplex ratio(1 to 64)
  IIC_writecmd(0x3F);//
  IIC_writecmd(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  IIC_writecmd(0xd3);//-set display offset
  IIC_writecmd(0x00);//-not offset
  IIC_writecmd(0xd5);//--set display clock divide ratio/oscillator frequency
  IIC_writecmd(0xf0);//--set divide ratio
  IIC_writecmd(0xd9);//--set pre-charge period
  IIC_writecmd(0x22); //
  IIC_writecmd(0xda);//--set com pins hardware configuration
  IIC_writecmd(0x12);
  IIC_writecmd(0xdb);//--set vcomh
  IIC_writecmd(0x20);//0x20,0.77xVcc
  IIC_writecmd(0x8d);//--set DC-DC enable
  IIC_writecmd(0x14);//
  IIC_writecmd(0xaf);//--turn on oled panel
  LCD_CLS();
  LCD_Set_Pos(0,0);
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void LCD_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size)
{      	
  unsigned char c=0,i=0;	
  c=chr-' ';//得到偏移后的值			
  if(x>Max_Column-1)
  {
     x=0;
     y=y+2;
  }
  if(Char_Size ==16)
  {
     LCD_Set_Pos(x,y);	
     for(i=0;i<8;i++)
     IIC_writedata(F8X16[c*16+i]);
     LCD_Set_Pos(x,y+1);
     for(i=0;i<8;i++)
     IIC_writedata(F8X16[c*16+i+8]);
   }
   else 
   {	
     LCD_Set_Pos(x,y);
     for(i=0;i<6;i++)
     IIC_writedata(F6x8[c][i]);			
   }
}


//显示一个字符号串
void LCD_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size)
{
   unsigned char j=0;
   while (chr[j]!='\0')
   {		
     LCD_ShowChar(x,y,chr[j],Char_Size);	
     x+=8;
     if(x>128)
     {
       x=0;
       y+=2;
     }
     j++;	
   }
}



//m^n函数
unsigned long oled_pow(unsigned char m,unsigned char n)
{
	unsigned long result=1;	 
	while(n--)result*=m;    
	return result;
}	



//显示一个数字
void LCD_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size2)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
}
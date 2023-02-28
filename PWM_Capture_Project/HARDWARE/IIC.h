#include 	"msp430f5529.h"


//IIC通信接口引脚输出宏定义
#define   SCL_HIGH    P6OUT|=BIT0
#define   SCL_LOW     P6OUT&=~BIT0
#define   SDA_HIGH    P6OUT|=BIT5
#define   SDA_LOW     P6OUT&=~BIT5


#define XLevelL           0x00
#define XLevelH           0x10
#define XLevel            ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column        128
#define Max_Row           64
#define Brightness        0xCF
#define X_WIDTH           128
#define Y_WIDTH           64



void IIC_delayus(unsigned int i);
void IIC_ioinit(void);
void IIC_start(void);
void IIC_stop(void);
void IIC_writebyte(unsigned char IIC_byte);
void IIC_writecmd(unsigned char IIC_command);
void IIC_writedata(unsigned char IIC_data);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_CLS(void);
void LCD_Init(void);
void LCD_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char Char_Size);
void LCD_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char Char_Size);
void LCD_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size2);
unsigned long oled_pow(unsigned char m,unsigned char n);
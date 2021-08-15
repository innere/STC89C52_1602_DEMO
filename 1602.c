#include <rg52.H>

#include <string.h>
#include <INTRINS.H>



/*
占用资源：
P0口,P1^2,P1^3,P1^5,P3^5;
*/

sbit lck = P3^5;//HC574锁存
sbit LCD_RS=P1^2;
sbit LCD_RW=P1^3;
sbit LCD_EN=P1^5;


void LCD_Delay()
{
	unsigned char i,j;
	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
			
}


/*******************************************************************/
/*                                                                 */                                                             
/*检查LCD忙状态                                                    */                                                       
/*lcd_busy为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据。      */ 
/*                                                                 */                                                                                                                                 
/*******************************************************************/





bit lcd_busy()
{                          
    bit result;
    lck = 0;
//    P1 = 0x28;//0x91;
	LCD_RS = 0; 
	LCD_RW = 1; 
	LCD_EN = 1;
    lck = 1; //锁存数据到HC574
    _nop_();
    _nop_();
    _nop_();
    _nop_();   
    result = (bit)(P0&0x80); 
    lck = 0;
//    P1 = 0x00;
	LCD_EN = 0;
    lck = 1;
    return result; 
}





/*******************************************************************/
/*                                                                 */                                                              
/*写指令数据到LCD                                                  */                                                     
/*RS=L，RW=L，E=上升沿，D0-D7=指令码。                             */  
/*                                                                 */                                                                                                                       
/*******************************************************************/

void LCD_WriteCommand(unsigned char cmd)
{                          
    while(lcd_busy());

    lck = 0;
//    P1 = 0x00;
  	LCD_RS = 0; 
	LCD_RW = 0; 
	LCD_EN = 0;
    lck = 1; 

    _nop_();
    _nop_(); 

    P0 = cmd;
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    lck = 0;
//    P1 = 0x20;  
	LCD_EN = 1;
    lck = 1;

    _nop_();
    _nop_();
    _nop_();
    _nop_();

    lck = 0;
//    P1 = 0x00;  
	LCD_EN = 0;  
    lck = 1;
}




/*******************************************************************/
/*                                                                 */                                                            
/*写显示数据到LCD                                                  */                                                      
/*RS=H，RW=L，E=上升沿，D0-D7=数据。                               */
/*                                                                 */                                                                                                                                
/*******************************************************************/

void LCD_WriteData(unsigned char dat) 
{                          
   while(lcd_busy());

	lck = 0;
	//    P1 = 0x04; 
	LCD_RS = 1;   
	LCD_RW = 0; 
	LCD_EN = 0;
	lck = 1; 

    P0 = dat;
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    lck = 0;
//    P1 = 0x24;// 
	LCD_EN = 1;
    lck = 1; 

    _nop_();
    _nop_();
    _nop_();
    _nop_();

    lck = 0;
//    P1 = 0x00;//
    LCD_EN = 0; 
    lck = 1; 
}
/**
  * @brief  LCD1602设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_WriteCommand(0x80|(Column-1));
	}
	else if(Line==2)
	{
		LCD_WriteCommand(0x80|(Column-1+0x40));
	}
}


/**
  * @brief  LCD1602初始化函数
  * @param  无
  * @retval 无
  */
void LCD_Init()
{
	LCD_WriteCommand(0x38);//八位数据接口，两行显示，5*7点阵
	LCD_WriteCommand(0x0c);//显示开，光标关，闪烁关
	LCD_WriteCommand(0x06);//数据读写操作后，光标自动加一，画面不动
	LCD_WriteCommand(0x01);//光标复位，清屏
}

/**
  * @brief  在LCD1602指定位置上显示一个字符
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符
  * @retval 无
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}

/**
  * @brief  在LCD1602指定位置开始显示所给字符串
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval SISIISI
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}

/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

/**
  * @brief  在LCD1602指定位置开始显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~65535
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-32768~32767
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column);
	if(Number>=0)
	{
		LCD_WriteData('+');
		Number1=Number;
	}
	else
	{
		LCD_WriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number1/LCD_Pow(10,i-1)%10+'0');
	}
}

/**
  * @brief  在LCD1602指定位置开始以十六进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFF
  * @param  Length 要显示数字的长度，范围：1~4
  * @retval 无
  */
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCD_WriteData(SingleNumber+'0');
		}
		else
		{
			LCD_WriteData(SingleNumber-10+'A');
		}
	}
}

/**
  * @brief  在LCD1602指定位置开始以二进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(2,i-1)%2+'0');
	}
}


#include <STC89C5xRC.H>
#include <string.h>
#include <INTRINS.H>



/*
ռ����Դ��
P0��,P1^2,P1^3,P1^5,P3^5;
*/

sbit lck = P3^5;//HC574����
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
/*���LCDæ״̬                                                    */                                                       
/*lcd_busyΪ1ʱ��æ���ȴ���lcd-busyΪ0ʱ,�У���дָ�������ݡ�      */ 
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
    lck = 1; //�������ݵ�HC574
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
/*дָ�����ݵ�LCD                                                  */                                                     
/*RS=L��RW=L��E=�����أ�D0-D7=ָ���롣                             */  
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
/*д��ʾ���ݵ�LCD                                                  */                                                      
/*RS=H��RW=L��E=�����أ�D0-D7=���ݡ�                               */
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
  * @brief  LCD1602���ù��λ��
  * @param  Line ��λ�ã���Χ��1~2
  * @param  Column ��λ�ã���Χ��1~16
  * @retval ��
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
  * @brief  LCD1602��ʼ������
  * @param  ��
  * @retval ��
  */
void LCD_Init()
{
	LCD_WriteCommand(0x38);//��λ���ݽӿڣ�������ʾ��5*7����
	LCD_WriteCommand(0x0c);//��ʾ�������أ���˸��
	LCD_WriteCommand(0x06);//���ݶ�д�����󣬹���Զ���һ�����治��
	LCD_WriteCommand(0x01);//��긴λ������
}

/**
  * @brief  ��LCD1602ָ��λ������ʾһ���ַ�
  * @param  Line ��λ�ã���Χ��1~2
  * @param  Column ��λ�ã���Χ��1~16
  * @param  Char Ҫ��ʾ���ַ�
  * @retval ��
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}

/**
  * @brief  ��LCD1602ָ��λ�ÿ�ʼ��ʾ�����ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~2
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ���
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
  * @brief  ����ֵ=X��Y�η�
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
  * @brief  ��LCD1602ָ��λ�ÿ�ʼ��ʾ��������
  * @param  Line ��ʼ��λ�ã���Χ��1~2
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~65535
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~5
  * @retval ��
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
  * @brief  ��LCD1602ָ��λ�ÿ�ʼ���з���ʮ������ʾ��������
  * @param  Line ��ʼ��λ�ã���Χ��1~2
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-32768~32767
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~5
  * @retval ��
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
  * @brief  ��LCD1602ָ��λ�ÿ�ʼ��ʮ��������ʾ��������
  * @param  Line ��ʼ��λ�ã���Χ��1~2
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~4
  * @retval ��
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
  * @brief  ��LCD1602ָ��λ�ÿ�ʼ�Զ�������ʾ��������
  * @param  Line ��ʼ��λ�ã���Χ��1~2
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
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

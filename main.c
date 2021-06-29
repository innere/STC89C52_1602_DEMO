#include <STC89C5xRC.H>
#include "Delay.h"
#include "1602.h"


unsigned int i;

//2021-6-29，回家验证

void main()
{
	LCD_Init();
//	LCD_ShowString(1,1,"aaaa");
	LCD_ShowString(1,1,"Zhang Xi Wei");
	LCD_ShowString(2,1,"Chi Pi+");
	while(1)
		{

			for(i=0;i<10000;i++)
				{
					//LCD_ShowString(1,1,"i");
					LCD_ShowNum(2,8,i,5);
					Delay(300);
				}
//			LCD_ShowString(1,1,"BBB");
//			LCD_ShowChar(2,1,'A');
//			LCD_ShowChar(2,1,"B");
			if (i>10000)
				{
					i=0;
				}
		}
}

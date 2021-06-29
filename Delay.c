#include <intrins.h>

void Delay(unsigned int xms)
{
	unsigned char i,j;
	while (xms--)
		{
			_nop_();
			i = 2;
			j = 199;
			do
			{
				while (--j);
			} while (--i);
			
		}
}

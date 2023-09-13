#include <LPC17xx.h>
main ()
{
	int i=0;
	int j=0;
	int c=0;
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;
	LPC_GPIO0->FIODIR |=0xFF<<4;
	LPC_PINCON->PINSEL4&=0xFCFFFFFF;
	LPC_GPIO2->FIODIR |= 0<<12;
	while(1)
	{
		if(LPC_GPIO2->FIOPIN & 1<<12)
		{
			if(c==256)
			{
				c=0;
			}
			c++;
		}
		else
		{
			if(c==0)
			{
				c=256;
			}
			c--;
		}
		LPC_GPIO0->FIOPIN=c<<(4);
		for(j=0;j<10000000;j++);
	}
}
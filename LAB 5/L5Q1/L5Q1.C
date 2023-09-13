#include <LPC17xx.h>
main ()
{
	int i=0;
	int j=0;
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL0 &= 0xFF0000FF;
	LPC_GPIO0->FIODIR |= 0x00000FF0;
	while(1)
	{
		for(i=0;i<256;i++)
		{
			LPC_GPIO0->FIOPIN=i<<(4);
			for(j=0;j<150000000;j++);
		}
	}
}
#include<LPC17xx.h>
#include<stdio.h>
#define FIRST_SEG 0xF87FFFFF
#define SECOND_SEG 0xF8FFFFFF
#define THIRD_SEG 0xF97FFFFF
#define FOURTH_SEG 0xF9FFFFFF
#define DISABLE_ALL 0xFA7FFFFF

unsigned int digit_1=0x00,digit_2=0x00,digit_3=0x00,digit_4=0x00;
unsigned int twenty_count=0x00,digit_count=0x00,dummy_1=0x00;
unsigned char numbers[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
unsigned char timer_flag=0x00,flag=0x00;
unsigned long int dummy_2=0x00000000,i=0;
unsigned int dummy_3=0x00;

void delay(void);
void display(void);

int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();

	LPC_PINCON->PINSEL0 &= 0XFF0000FF;
	LPC_PINCON->PINSEL3 &= 0XFFC03FFF;
	LPC_PINCON->PINSEL4 &= 0XFFFFFFFC;
	LPC_GPIO0->FIODIR |= 0X00000FF0;
	LPC_GPIO1->FIODIR |= 0X07800000;
	LPC_GPIO2->FIODIR |= 0xFFFFFFFE;

	while(1)
	{
		delay();
		digit_count++;
		if(digit_count==0x05)
		{
			digit_count=0x00;
			flag=0xFF;
		}
		if(flag==0xFF)
		{
			flag=0x00;
			if((LPC_GPIO2->FIOPIN & 1))
			{
				digit_1++;
				if(digit_1==0x0A)
				{
					digit_1=0;
					digit_2++;
					if(digit_2==0x0A)
					{ 
						digit_2=0;
						digit_3++;
						if(digit_3==0x0A)
						{
							digit_3=0;
							digit_4++;
							if(digit_4==0x0A)
							{
								digit_4=0;
							}
						}
					}
				}
			}
			else
			{
				digit_1--;
				if(digit_1==0x0)
				{
					digit_1=0x9;
					digit_2--;
					if(digit_2==0x0)
					{ 
						digit_2=0x9;
						digit_3--;
						if(digit_3==0x0)
						{
							digit_3=0x9;
							digit_4--;
							if(digit_4==0x0)
							{
								digit_4=0x9;
								digit_3=0x9;
								digit_2=0x9;
								digit_1=0x9;
							}
						}
					}
				}
			}
		}
		display();
	}
}

void display(void)
{
	if(digit_count==0x01)
	{
		dummy_1=digit_1;
		LPC_GPIO1->FIOPIN=FIRST_SEG;
	}
	
	else if(digit_count==0x02)
	{
		dummy_1=digit_2;
		LPC_GPIO1->FIOPIN=SECOND_SEG;
	}
	
	else if(digit_count==0x03)
	{
		dummy_1=digit_3;
		LPC_GPIO1->FIOPIN=THIRD_SEG;
	}
	
	else if(digit_count==0x04)
	{
		dummy_1=digit_4;
		LPC_GPIO1->FIOPIN=FOURTH_SEG;
	}
	
	dummy_1 &= 0x0f;
	dummy_2 = numbers[dummy_1];
	dummy_2 = dummy_2<<4;
	LPC_GPIO0->FIOPIN = dummy_2;
	
	for(i=0;i<500;i++);
	
	LPC_GPIO0->FIOCLR = 0xff0;
	LPC_GPIO1->FIOPIN = DISABLE_ALL;
}

void delay(void)
{
	unsigned int i;
	
	for(i=0;i<100;i++);
	
	if(twenty_count==2000)
	{
		flag=0xff;
		twenty_count=0x00;
	}
	
	else
	{
		twenty_count += 1;
	}
}

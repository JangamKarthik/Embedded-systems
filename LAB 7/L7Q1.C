#include<LPC17xx.h>
#include<stdlib.h>
int temp1,temp2,flag1,i;
unsigned char k;
void port_write(){
	int j;
	LPC_GPIO0->FIOPIN = temp2<<23;
	if(flag1==0){
		LPC_GPIO0->FIOCLR=1<<27;
	}
	else{
		LPC_GPIO0->FIOSET=1<<27;
	}
	LPC_GPIO0->FIOSET=1<<28;
	for(j=0;j<50;j++);
	LPC_GPIO0->FIOCLR=1<<28;
	for(j=0;j<10000;j++);
}
void lcd_write(){
	temp2=(temp1>>4)&0xF;
	port_write();
	temp2 = temp1& 0xF;
	port_write();
}

 

int main(){
	int command[] = {3,3,3,2,2,0x01,0x06,0x0C,0x80};
	char message[]="message1";
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL1 = 0;
	LPC_GPIO0->FIODIR=0XF<<23| 1 <<27 | 1<<28;
	LPC_PINCON->PINSEL4  = 0<<12;
	LPC_GPIO2->FIODIR |= 0x00;
	flag1=0;
	for(i=0;i<9;i++)
        {temp1 = command[i]; lcd_write();}
	flag1=1;
	i=0;
while(1)
	{
		if(!(LPC_GPIO2->FIOPIN & 1<<12))
		{
			k = (rand()%6)+1;
			k=k+0x30;
			temp1=k;
            lcd_write();
			for(i = 0; i<10000000;i++){};
			flag1 = 0;
			temp1 = 0x01;
			lcd_write();
			flag1 = 1;
		}
	}
}

#include <lpc17xx.h>

#define RS_CTRL  0x08000000  //P0.27, 1<<27
#define EN_CTRL  0x10000000  //P0.28, 1<<28
#define DT_CTRL  0x07800000  //P0.23 to P0.26 data lines, F<<23

 unsigned long int temp1=0, temp2=0,i,j,r;
 unsigned char flag1 =0, flag2 =0,key;
 unsigned char finans[2]={'0','0'};
 unsigned int row,col,flag,x;
 int a,b,count=0,ans,idx=1;
 char op;

void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned int);
unsigned long int init_command[] = {0x30,0x30,0x30,0x20,0x28,0x01 ,0x06,0x0c,0x80};
unsigned char ascii[4][4]={{'0','1','2','3'},{'4','5','6','7'},{'8','9','+','-'},{'0','1','2','3'}};
 int main(void)
 {
	SystemInit();
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL1=0;
  LPC_PINCON->PINSEL4=0;
  LPC_PINCON->PINSEL3=0;
  LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL; //0xf<<23 | 1<<27 | 1<<28;
  LPC_GPIO2->FIODIR = 0XF<<10;
    LPC_GPIO1->FIODIR =0;
  flag1 =0;
	for (i=0; i<9;i++)
                    {
	    temp1 = init_command[i];
    	    lcd_write();
                    }
  flag1 =1; //DATA MODE
	
	

	while(count<3){
    for (row=0;row<4;row++)
				{		flag=0;
						LPC_GPIO2->FIOPIN = 1<<(10+row);
                        //sending high on ith row
                        for(j=0;j<100;j++);

					  x = ((LPC_GPIO1->FIOPIN>>23) & 0xf); //CHECK IF ANY COLUMN LINE IS HIGH

						if(x)
						{
                               //some key is pressed
							  if (x == 1)  //finding col no.
									col = 0;
								else if (x ==2)
								   col=1;
								else if (x ==4)
								   col =2;
								else if (x ==8)
								   col =3;
																count++;
                                key=ascii[row][col];
                                temp1=key;
                                lcd_write();
                                if(count==1){
                                    a=key-0x30;
                                }
                                else if(count==2){
                                    op=key;
                                }
                                else if(count==3){
                                    b=key-0x30;
                                }
                        }
								for(i=0;i<500000;i++);
												
  }
 }
 temp1=0xC0;
 flag1=0;
 lcd_write();
 flag1=1;
 switch(op){
    case '+':
        ans=a+b;
        break;
    case '-':
        ans=a-b;
        break;
 }
 while(ans!=0){
    finans[idx--]=(ans%10)+0x30;
    ans=ans/10;
 }
 for(i=0;i<2;i++){
    temp1=finans[i];
    lcd_write();
 }
 }

  void lcd_write(void)
                 { 
                  temp2 = temp1 & 0xf0;// 4 - Bits to get it to least significant digit place
	temp2 = temp2>>4;
	port_write();
                 if (!((flag1==0)&&((temp1==0x20)||(temp1==0x30)))) //send least significant 4 bits only when it is data/command other than 0x30/0x20
                  {
	   temp2 = temp1 & 0x0f; 
	   temp2 = temp2 ;
	   port_write();
                   }
                 }


 void port_write(void)

 {
	
	LPC_GPIO0->FIOPIN = temp2<<23; // sending the ascii code
          if (flag1 == 0)
                  LPC_GPIO0->FIOCLR = RS_CTRL; // if command
          else
                  	LPC_GPIO0->FIOSET = RS_CTRL; //if data

	LPC_GPIO0->FIOSET = EN_CTRL; //sending a low high edge on enable input
	for(r=0;r<25;r++);
	LPC_GPIO0->FIOCLR = EN_CTRL;
    for(r=0;r<30000;r++);
}

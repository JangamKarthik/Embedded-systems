#include<LPC17xx.h>

#define Ref_Vtg 3.3000

#define Full_Scale 4096

unsigned long x, temp, t1 = 0, t2 = 0;

unsigned int channel, result, i, temp1, j;

unsigned char Msg1[15] ={"ADC Value"};

void lcd_init(void);

void write(int, int);

void delay_lcd(unsigned int);

void lcd_comdata(int, int);

void clear_ports(void);

void lcd_puts(unsigned char *);

 

int main(void){

    unsigned long adc_temp;

    unsigned int i;

    float in_vtg;

    unsigned char vtg[7], dval[7];

    unsigned char Msg3[11] = {"ANALOG:"};

    unsigned char Msg4[12] = {"ADC:"};

    SystemInit();

    SystemCoreClockUpdate();

    LPC_SC->PCONP |= (1<<15); //Power for GPIO block

    lcd_init();

    LPC_PINCON->PINSEL3 |= 0xC0000000; //P1.31 as AD0.5

    LPC_SC->PCONP |= (1<<12); //enable the peripheral ADC

    SystemCoreClockUpdate();

    lcd_comdata(0x80, 0);

    delay_lcd(800);

    lcd_puts(&Msg3[0]);

    lcd_comdata(0xC0, 0);

    delay_lcd(800);

    lcd_puts(&Msg4[0]);

    while(1){

        LPC_ADC->ADCR = (1<<5)|(1<<21)|(1<<24);

        while(!(LPC_ADC->ADGDR >>31 & 1)); //waiting till done bit = 1

        

        adc_temp = LPC_ADC->ADDR5;

        adc_temp >>= 4; //extracting result

        adc_temp &= 0x00000FFF;

        in_vtg = (((float)adc_temp * (float)Ref_Vtg))/((float)Full_Scale);

        //calculating input analog voltage

        sprintf(vtg, "%3.2fV", in_vtg);

        //convert the readings into string to display on LCD

        sprintf(dval, "%x", adc_temp);

        for(i=0; i<2000; i++);

        lcd_comdata(0x89, 0);

        delay_lcd(800);

        lcd_puts(&vtg[0]);

        lcd_comdata(0xC8, 0);

        delay_lcd(800);

        lcd_puts(&dval[0]);

        for(i=0;i<200000;i++);

        for(i=0;i<7;i++)

            vtg[i] = dval[i] = 0x00;

        adc_temp = 0;

        in_vtg = 0;

    }

}

 

void lcd_init(){

    LPC_PINCON->PINSEL1 &= 0xFC003FFF;

 

    LPC_GPIO0->FIODIR |= 0x0F<<23 | 1<<27 | 1<<28;

 

    clear_ports();

    delay_lcd(3200);

    lcd_comdata(0x33, 0);

    delay_lcd(30000);

    lcd_comdata(0x32, 0);

    delay_lcd(30000);

    lcd_comdata(0x28, 0); //function set

    delay_lcd(30000);

    lcd_comdata(0x0c, 0);//display on cursor off

    delay_lcd(800);

    lcd_comdata(0x06, 0); //entry mode set increment cursor right

    delay_lcd(800);

    lcd_comdata(0x01, 0); //display clear

    delay_lcd(10000);

    return;

}

 

void lcd_comdata(int temp1, int type){

    int temp2 = temp1 & 0xf0; //move data (26-8+1) times : 26 - HN place, 4 - Bits

    temp2 = temp2 << 19; //data lines from 23 to 26

    write(temp2, type);

    temp2 = temp1 & 0x0f; //26-4+1

    temp2 = temp2 << 23;

    write(temp2, type);

    delay_lcd(1000);

    return;

}

    

void write(int temp2, int type){

    clear_ports();

    LPC_GPIO0->FIOPIN = temp2;

    if(type==0)

        LPC_GPIO0->FIOCLR = 1<<27;

    else

        LPC_GPIO0->FIOSET = 1<<27; // set bit RS for Data

    LPC_GPIO0->FIOSET = 1<<28; // EN=1

    delay_lcd(25);

    LPC_GPIO0->FIOCLR = 1<<28; // EN =0

    return;

}

void delay_lcd(unsigned int r1){

unsigned int r;

for(r=0;r<r1;r++);

return;

}

 

void clear_ports(void){

    LPC_GPIO0->FIOCLR = 0x0F<<23; //Clearing data lines

    LPC_GPIO0->FIOCLR = 1<<27; //Clearing RS line

    LPC_GPIO0->FIOCLR = 1<<28; //Clearing Enable line

 

    return;

}

 

void lcd_puts(unsigned char *buf1){

    unsigned int i=0;

    unsigned int temp3;

    while(buf1[i]!='\0'){

        temp3 = buf1[i];

        lcd_comdata(temp3, 1);

        i++;

        if(i==16){

            lcd_comdata(0xc0, 0);

        }

    }

    return;

}

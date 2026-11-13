#include<LPC21XX.H>
#include<string.h>
#define lcd_d 0xf<<10
#define rs 1<<8
#define e 1<<9
#define green_led 1<<6
#define red_led 1<<7

#define pump_1 1<<5
#define pump_2 1<<4
#define moist_sensor 1<<3

unsigned char buffer[100];
volatile unsigned char i = 0;
volatile unsigned int status=0,function=0,current_state=0,previous_state=2;

void lcd_init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char*);
void delay_seconds(unsigned int);
void delay_milliseconds(unsigned int);
void uart0_config(void);
void uart0_tx(unsigned char);
unsigned char uart0_rx(void);
void uart0_string_tx(unsigned char *);
void gsm_receive_message(void);
void gsm_send_msg(unsigned char *);
void pump_on(void);
void pump_off(void);

void isr(void) __irq
{
    unsigned char iir=U0IIR;
    
   
    if ((iir&0x0E)==0x04)
    {
        char ch=U0RBR;
		if(ch=='*')
			i=0;
		else if(ch=='#')
		{
			buffer[i]='\0';
			status=1;
			i=0;
		}
		else 
		{
			if(i < 99) 
				buffer[i++] = ch;
			else
				i=0;
    	}
	}
    VICVectAddr=0; 
}

int main()
{  
	IODIR0|=green_led|red_led|pump_1|pump_2;
	uart0_config();
	
        VICIntSelect = 0x00000000; 
	VICVectAddr0 = (int)isr;
	VICVectCntl0 = (1<<5)|6;  
	VICIntEnable = (1<<6);      
	U0IER=0x01;

	gsm_receive_message();	
	lcd_init();

	IOSET0=green_led|red_led;
	pump_off();

	lcd_command(0x80);
	lcd_string("soil moisture based plant watering system ");
	delay_seconds(3);

	while(1)
	{
		if(status)
		{
			status=0;
			if((strstr((char*)buffer,"STATUS"))!=NULL)
			{
				if(function)
				{
					gsm_send_msg("SOIL-WET,WATERING DONE");
					delay_milliseconds(500);
				}
				else
				{
					gsm_send_msg("SOIL-DRY,WATERING STARTED");
					delay_milliseconds(500);
				}
			}
		}
		
		{
			if((IOPIN0&moist_sensor)==0)
				current_state=1;
			else
				current_state=0;
			
			if(current_state!=previous_state)
			{
				previous_state=current_state;
				if(current_state==1)
				{
					function=1;
					IOSET0=red_led;
					IOCLR0=green_led;
					pump_off();
					lcd_string("SOIL-WET,WATERING DONE");
					gsm_send_msg("SOIL-WET,WATERING DONE");
				}
				else
				{
					function=0;
					IOSET0=green_led;
					IOCLR0=red_led;
					pump_on();
					lcd_string("SOIL-DRY,WATERING STARTED");
					gsm_send_msg("SOIL-DRY,WATERING STARTED");
				}
			}	
		} 
	}
}

void lcd_init(void)
{
	IODIR0|=lcd_d|rs|e;
	lcd_command(0x01);
	lcd_command(0x02);
	lcd_command(0x0c);
	lcd_command(0x28);

}
void lcd_command(unsigned char c)
{
	IOCLR0=lcd_d;
	IOSET0=(c&0xf0)<<6;
	IOCLR0=rs;
	IOSET0=e;
	delay_milliseconds(2);
	IOCLR0=e;

	IOCLR0=lcd_d;
	IOSET0=(c&0x0f)<<10;
	IOCLR0=rs;
	IOSET0=e;
	delay_milliseconds(2);
	IOCLR0=e;
}
void lcd_data(unsigned char d)
{
	IOCLR0=lcd_d;
	IOSET0=(d&0xf0)<<6;
	IOSET0=rs;
	IOSET0=e;
	delay_milliseconds(2);
	IOCLR0=e;

	IOCLR0=lcd_d;
	IOSET0=(d&0x0f)<<10;
	IOSET0=rs;
	IOSET0=e;
	delay_milliseconds(2);
	IOCLR0=e;
}
void lcd_string(unsigned char *s)
{
	
	unsigned int pos=0;
	lcd_command(0x01);
	while(*s)
	{
		lcd_data(*s++);
		pos++;
		if(pos==16)
		lcd_command(0xC0);
	}
}

void delay_milliseconds(unsigned int ms)
{
	T0PR=15000-1;
	 T0TCR=0x01;
	 while(T0TC<ms);
	 T0TCR=0x03;
	 T0TCR=0x00; 
}
void delay_seconds(unsigned int s)
{
	 T0PR=15000000-1;
	 T0TCR=0x01;
	 while(T0TC<s);
	 T0TCR=0x03;
	 T0TCR=0x00;  
} 

void uart0_config(void)
{
	PINSEL0|=0x05;
	U0LCR=0x83;
	U0DLL=97;
	U0DLM=0;
	U0LCR=0x03;
}

void uart0_tx(unsigned char c)
{
	while(((U0LSR>>5)&1)==0);
	U0THR=c;	
}

unsigned char uart0_rx(void)
{
	while((U0LSR&0x01)==0);
	return U0RBR;
}

void uart0_string_tx(unsigned char *s)
{
	while(*s)
	uart0_tx(*s++);
}

void gsm_receive_message(void)
{
	uart0_string_tx("AT\r\n");
	delay_milliseconds(500);
	uart0_string_tx("AT+CMGF=1\r\n");
	delay_milliseconds(500);
	uart0_string_tx("AT+CNMI=2,2,0,0,0\r\n");
	delay_milliseconds(500);
} 

void gsm_send_msg(unsigned char *s)
{
	uart0_string_tx("AT\r\n");
	delay_milliseconds(500);
	uart0_string_tx("AT+CMGF=1\r\n");
	delay_milliseconds(500);
	uart0_string_tx("AT+CMGS=\"+919842588997\"\r\n");
	delay_milliseconds(500);	
	uart0_string_tx(s);
	delay_milliseconds(500);
	uart0_tx(26);
	delay_milliseconds(500);
}

void pump_on(void)
{
	IOSET0=pump_1;
	IOCLR0=pump_2;	
}
void pump_off(void)
{
	IOSET0=pump_1|pump_2;
}

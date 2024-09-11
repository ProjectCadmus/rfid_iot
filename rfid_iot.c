#define F_CPU 8000000UL
#include<avr/io.h>
#include<util/delay.h>
#define RS 0
#define RW 1
#define E 2
#define lcd_port PORTC

void lcd_init();
void lcd_cmd(unsigned char a);
void lcd_data(unsigned char a);
void dis_cmd(unsigned char a);
void dis_data(unsigned char a);
void dis_string(unsigned char *str);
void dis_number(long data);
void uart_init();
int uart_recieve();
void uart_transmit(char data);
int send_sms(unsigned char *str,unsigned char *str1);     //FOR SENDING SMS
char ch[12];


void main()
{
	DDRC=0xff;     //for lcd
	DDRB=0xff;     //status leds
	lcd_init();
	uart_init();
	
	dis_cmd(1);
	_delay_ms(200);
	dis_cmd(0x80);
	dis_string("WELCOME");
	dis_cmd(0xC0);
	dis_string("PRANJAL");
	_delay_ms(2000);

	dis_cmd(1);
	_delay_ms(200);


	

	
	
	while(1)
	{
		dis_cmd(0x80);
		dis_string("PLEASE SHOW YOUR");
		dis_cmd(0xC0);
		dis_string("CARD TO ENTER   ");
		_delay_ms(200);

		int user=user_match();

		if(user==0)
		{
			dis_cmd(0xc0);
			dis_string("No Match Found");
			_delay_ms(2000);
			
			dis_cmd(1);
			_delay_ms(1);
			PORTB&=~(1<<0);

		}
		if(user==1)
		{
			dis_cmd(0xc0);
			dis_string("WELCOME KARAN");
			_delay_ms(2000);
			//send_sms("9871332410","Rohit Entered the Campus");_delay_ms(200);
			dis_cmd(1);
			_delay_ms(1);
			PORTB&=~(1<<0);
		}
		if(user==2)
		{
			dis_cmd(0xc0);
			dis_string("WELCOME KAPIL ");
			_delay_ms(2000);
			//send_sms("9871332410","Amit Entered the Campus");_delay_ms(200);
			dis_cmd(1);
			_delay_ms(1);
			PORTB&=~(1<<0);
		}
 		if(user==3)
		{
			dis_cmd(0xc0);
			dis_string("ALERT");
			_delay_ms(2000);
			//send_sms("8802845020","ATTENTION:SOMEONE ENTERED");_delay_ms(200);
			dis_cmd(1);
			_delay_ms(1);
			PORTB&=~(1<<0);
		}

		
	


	}
}

void card_info() // FOR RECEIVING 12 DIGIT RFID NO
{
	int num=0;
	int end=0;
	while(num<12)
	{
		while((UCSRA&(1<<RXC))==0);
		ch[end]=UDR;
		end++;
		num++;
	}

	num=0;   
	dis_cmd(1);
	_delay_ms(100);

	for(int i=0;i<end;i++)   //// FOR DISPLAYING 12 DIGIT RFID NO
	{
		dis_data(ch[i]);
	}
	end=0;
}

int info_cmp(char *data, char *str)
{
	
	int count=0;
	PORTB|=(1<<0);
	for(int i=0; i<=11; i++)
	{
		if(data[i]==str[i])
		count++;
	}
	
	if(count==12)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

int user_match()
{

	int result=0;
	card_info();	

	result=info_cmp(ch,"0B0023F8A171");
	if(result==1)
	{return(1);}	

	result=info_cmp(ch,"0B0023EA37F5");
	if(result==1)
	{return(2);}

	result=info_cmp(ch,"0B0023E9E223");
	if(result==1)
	{return(3);}

	result=info_cmp(ch,"0B0024057953");
	if(result==1)
	{return(4);}

	
	return(0); 
		
}

void uart_init()
{	UCSRB|=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	UCSRC|=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UBRRL=51;
}

int uart_recieve()
{
	while((UCSRA&(1<<RXC))==0);
	return(UDR);
}

void uart_transmit(char data)
{	while((UCSRA&(1<<UDRE))==0);
	UDR=data;
}

void lcd_init()
{
	dis_cmd(0x02);
	dis_cmd(0x28);
	dis_cmd(0x0D);
	dis_cmd(0x06);
}

void lcd_cmd(unsigned char a)
{
	lcd_port=a;
	lcd_port &=~(1<<RS);
	lcd_port &=~(1<<RW);
	lcd_port |=(1<<E);
	_delay_ms(1);
	lcd_port &=~(1<<E);
}

void lcd_data(unsigned char a)
{
	lcd_port=a;
	lcd_port |=(1<<RS);
	lcd_port &=~(1<<RW);
	lcd_port |=(1<<E);
	_delay_ms(1);
	lcd_port &=~(1<<E);
}

void dis_cmd(unsigned char a)
{
	unsigned char a1=a&0xF0;
	lcd_cmd(a1);
	a1=(a<<4)&0xF0;
	lcd_cmd(a1);
}

void dis_data(unsigned char a)
{
	unsigned char a2=a&0xF0;
	lcd_data(a2);
	a2=(a<<4)&0xF0;
	lcd_data(a2);
}
void dis_string(unsigned char *str)
{
	int i=0;
	while(str[i]!= '\0')
	{
		dis_data(str[i]);
		i++;
	}
}
void dis_number(long data)
{
	unsigned int c;
	dis_cmd(0X04);
	while(data!=0)
	{
		c=data%10;
		dis_data(c+48);
		data=data/10;
	}
	dis_cmd(0X06);
}

int send_sms(unsigned char *str,unsigned char *str1)
{
	PORTB|=(1<<1);
	uart_transmit('A');
	uart_transmit('T');
	uart_transmit('+');
	uart_transmit('C');
	uart_transmit('M');
	uart_transmit('G');
	uart_transmit('F');
	uart_transmit('=');
	uart_transmit('1');
	uart_transmit(0x0d);
	_delay_ms(1000);
	uart_transmit('A');
	uart_transmit('T');
	uart_transmit('+');
	uart_transmit('C');
	uart_transmit('M');
	uart_transmit('G');
	uart_transmit('S');
	uart_transmit('=');
	int i=0;
	uart_transmit('"');
	while(str[i]!= '\0')
	{
		uart_transmit(str[i]);
		i++;
	}
	//uart_transmit('\0');
	uart_transmit('"');
	uart_transmit(';');
	uart_transmit(0x0d);
	 i=0;
	 _delay_ms(2000);
	while(str1[i]!= '\0')
	{
		uart_transmit(str1[i]);
		i++;
	}
	uart_transmit(0x1a);
	uart_transmit(0x0D);
	PORTB&=~(1<<1);
	return(0);
}


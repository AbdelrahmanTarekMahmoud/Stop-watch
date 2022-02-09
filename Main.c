// Mini project 2 (Stop Watch)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
char pin[6];
unsigned char flag;
void UPDATE(void)
{

	pin[0]++;

if (pin[0]==10)
{
	pin[1]++;
	pin[0]=0;
	if(pin[1] == 6)
	{
		pin[2]++;
		pin[1] = 0;
	}
}

if (pin[2] == 10)
{
	pin[3]++;
	pin[2]=0;
	if (pin[3] == 6)
	{
		pin[4]++;
		pin[3]=0;
	}
}
if (pin[4] == 10)
{
	pin[5]++;
	pin[4]=0;
	if (pin[5] == 6)
	{
		pin[0]=0;
		pin[1]=0;
		pin[2]=0;
		pin[3]=0;
		pin[4]=0;
		pin[5]=0;
	}
}

}
void Timer1()
{

TCCR1A |= (1<<FOC1A);
TCCR1B |= (1<<CS12) | (1<<CS10) |(1<<WGM12);
TCNT1 = 0;
OCR1A = 1000;
TIMSK |= (1<<OCIE1A);

}

ISR(TIMER1_COMPA_vect)
{
SREG |= (1<<7);
flag =1;
}

void INT0_Init(void)
{
DDRD &=~(1<<PD2);
PORTD |=(1<<PD2);
GICR |= (1<<INT0);
MCUCR |=(1<<ISC01);
}

ISR(INT0_vect)
{
SREG |=(1<<7);
for (int i = 0 ;i<6;i++)
{
	pin[i] =0;
}
TCNT1=0;
}

void INT1_Init(void)
{
	DDRD &=~(1<<PD3); // PD3 input
	MCUCR |=(1<<ISC11) |(1<<ISC10); // int1 rising edge
	GICR |= (1<<INT1);
}

ISR(INT1_vect)
{
SREG|=(1<<7);
TCCR1B &=~(1<<CS10);
TCCR1B &=~(1<<CS11);
TCCR1B &=~(1<<CS12);
}

void INT2_Init(void)
{
	DDRB |= (1<<PB2);
	PORTB |= (1<<PB2);
	MCUCSR &=~ (1<<ISC2);
	GICR |= (1<<INT2);
}

ISR(INT2_vect)
{
	SREG |=(1<<7);
	TCCR1B |= (1<<CS12) | (1<<CS10);
}


void show()
{
	for (int i =0 ;i<6;i++)
	{
		PORTA=(1<<i);
		PORTC = pin[i];
		_delay_ms(2);
	}
}

int main()
{
    SREG |=(1<<7);
	DDRC = 0x0F;  //4 pins in Portc are outputs
	PORTC = 0x00; // initial value of PC0 to show in decoder
	DDRA = 0x3F; // first 6 pins in PORTA are outputs
	PORTA = 0x01; // Show PA0 as initial pin in 7 segment
	INT0_Init();
	INT1_Init();
	INT2_Init();
	Timer1();

	while (1)
	{
      show();
      if (flag == 1)
      {
    	  UPDATE();
    	  flag=0;
      }

	}
}


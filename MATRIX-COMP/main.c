/*
 * MATRIX-COMP.c
 *
 * Created: 21-03-2018 16:29:10
 * Author : nived
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>

// global variable to count the number of overflows
volatile uint8_t tot_overflow;


#define  F_CPU 14745600UL
#include <util/delay.h>
#define  BAUDRATE 9600
#define BAUD_PRESCALER ((F_CPU/16*(BAUDRATE))-1)

void USART_Init( unsigned int ubrr)
{
	//Set baud rate
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	UCSRB = (1<<RXEN)|(1<<TXEN);
	// Set frame format: 8data, 2stop bit
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
	// Wait for empty transmit buffer
	while ( !( UCSRA & (1<<UDRE)) );
	// Put data into buffer, sends the data
	UDR = data;
}

/*
void USART_Transmitf(float s){
	
	char chptr = (unsigned char *) &s;
	USART_Transmit(*chptr++);
	USART_Transmit(*chptr++);
	USART_Transmit(*chptr++);
	USART_Transmit(*chptr);
	
	
	
}

*/

void USART_TransmitF(float s){
	//s = 4.444444444;
	if(s<0) s = s*(-1);
	
	while(1){
		USART_Transmit('a');
		_delay_ms(100);
		uint32_t a = (uint32_t)(s*10000000);
		//a = 17382;
		
		if(s<0.0000001)USART_Transmit('q');
		if(s<0.000001)USART_Transmit('z');
		if(s<0.00001)USART_Transmit('d');
		/*
		else{
			a = a/100;
			USART_Transmit((unsigned char)(a%10 + 48));
		}
		*/
		
		while(a!=0){
			
			unsigned char f = (unsigned char)(a-10*(a/10) + 48);
			a = a/10;
			//a = 44;
			USART_Transmit(f);
			_delay_ms(150);
			USART_Transmit('w');
			_delay_ms(100);
			
		}
	}
	
}



void timer0_init()
{
	// set up timer with no prescaling
	TCCR0 |= (0 << CS02);
	TCCR0 |= (0 << CS01);
	TCCR0 |= (1 << CS00);
	
	TIMSK |= (1 << TOIE0);
	
	// enable global interrupts
	sei();
	
	// initialize counter
	TCNT0 = 0;
	
	//DDRC |= (1 << 0);
	tot_overflow = 0;
	
	
}

void timer1_init()
{
	// set up timer with prescaler = 8
	TCCR1B |= (0 << CS10);
	TCCR1B |= (0 << CS11);
	TCCR1B |= (1 << CS12);
	
	// initialize counter
	TCNT1 = 0;
	
	
	TIMSK |= (1 << TOIE1);
	
	// enable global interrupts
	sei();
	
	// initialize overflow counter variable
	tot_overflow = 0;
	
}



// called whenever TCNT1 overflows
ISR(TIMER0_OVF_vect)
{
	// keep a track of number of overflows
	tot_overflow++;
	
}






int main()
{
	
	USART_Init(95);
	
	timer0_init();
    float **input, **Inverse, *temp, localVariable, app;
    uint8_t i, j, k, sizeOfMatrix, lindex;
	USART_Transmit('a');

    //printf("Enter matrix size. It's a square matrix. So enter value of n (nXn)\n"); //CodewithC.com
    //scanf_s("%d", &sizeOfMatrix);

    sizeOfMatrix = 3;

    input = (float **)malloc(sizeOfMatrix * sizeof(float *));

    for (i = 0; i<sizeOfMatrix; i++)
        input[i] = (float *)malloc(sizeOfMatrix * sizeof(float));

    Inverse = (float **)malloc(sizeOfMatrix * sizeof(float *));
	temp = (float*)malloc(sizeOfMatrix * sizeof(float));


    for (i = 0; i<sizeOfMatrix; i++)
    {

        Inverse[i] = (float *)malloc(sizeOfMatrix * sizeof(float));
    }
	USART_Transmit('b');
	/*
	input[0][0] = 4;
	input[0][1] = 2;
	input[1][0] = 7;
	input[1][1] = 3;
	*/
	USART_Transmit('c');
	_delay_ms(100);
	
    input[0][0] = 1;
    input[0][1] = 8;
    input[0][2] = 7;
    input[1][0] = 4;
    input[1][1] = 0;
    input[1][2] = 6;
    input[2][0] = 5;
    input[2][1] = 8;
    input[2][2] = 5;
	
	USART_Transmit('y');
	
	/*
	input[0][0] = 1;
	input[0][1] = 0;
	input[0][2] = 0;
	input[1][0] = 0;
	input[1][1] = 1;
	input[1][2] = 0;
	input[2][0] = 0;
	input[2][1] = 0;
	input[2][2] = 1;
	
	
	input[0][0] = 2;
	input[0][1] = 5;
	input[0][2] = 9;
	input[0][3] = 4;
	input[1][0] = 3;
	input[1][1] = 0;
	input[1][2] = 5;
	input[1][3] = 1;
	input[2][0] = 8;
	input[2][1] = 7;
	input[2][2] = 46;
	input[2][3] = 3;
	input[3][0] = 0;
	input[3][1] = 4;
	input[3][2] = 9;
	input[3][3] = 3;
	
	
	
	input[0][0] = 8;
	input[0][1] = 9;
	input[0][2] = 5;
	input[0][3] = 3;
	input[0][4] = 0;
	input[1][0] = 2;
	input[1][1] = 1;
	input[1][2] = 9;
	input[1][3] = 0;
	input[1][4] = 5;
	input[2][0] = 8;
	input[2][1] = 9;
	input[2][2] = 5;
	input[2][3] = 2;
	input[2][4] = 8;
	input[3][0] = 7;
	input[3][1] = 5;
	input[3][2] = 4;
	input[3][3] = 2;
	input[3][4] = 1;
	input[4][0] = 8;
	input[4][1] = 5;
	input[4][2] = 7;
	input[4][3] = 6;
	input[4][4] = 1;
	
	
	
	
	input[0][0] = 8;
	input[0][1] = 9;
	input[0][2] = 5;
	input[0][3] = 3;
	input[0][4] = 0;
	input[0][5] = 7;
	input[1][0] = 2;
	input[1][1] = 1;
	input[1][2] = 9;
	input[1][3] = 0;
	input[1][4] = 5;
	input[1][5] = 3;
	input[2][0] = 8;
	input[2][1] = 9;
	input[2][2] = 5;
	input[2][3] = 2;
	input[2][4] = 8;
	input[2][5] = 0;
	input[3][0] = 7;
	input[3][1] = 5;
	input[3][2] = 4;
	input[3][3] = 2;
	input[3][4] = 1;
	input[3][5] = 9;
	input[4][0] = 8;
	input[4][1] = 5;
	input[4][2] = 7;
	input[4][3] = 6;
	input[4][4] = 1;
	input[4][5] = 34;
	input[5][0] = 0;
	input[5][1] = 7;
	input[5][2] = 4;
	input[5][3] = 2;
	input[5][4] = 90;
	input[5][5] = 2;
	
	
	
	
	input[0][0] = 1;
	input[0][1] = 0;
	input[0][2] = 0;
	input[0][3] = 0;
	input[0][4] = 0;
	input[0][5] = 0;
	input[1][0] = 0;
	input[1][1] = 1;
	input[1][2] = 0;
	input[1][3] = 0;
	input[1][4] = 0;
	input[1][5] = 0;
	input[2][0] = 0;
	input[2][1] = 0;
	input[2][2] = 1;
	input[2][3] = 0;
	input[2][4] = 0;
	input[2][5] = 0;
	input[3][0] = 0;
	input[3][1] = 0;
	input[3][2] = 0;
	input[3][3] = 1;
	input[3][4] = 0;
	input[3][5] = 0;
	input[4][0] = 0;
	input[4][1] = 0;
	input[4][2] = 0;
	input[4][3] = 0;
	input[4][4] = 1;
	input[4][5] = 0;
	input[5][0] = 0;
	input[5][1] = 0;
	input[5][2] = 0;
	input[5][3] = 0;
	input[5][4] = 0;
	input[5][5] = 1;
	*/
	USART_Transmit('d');
	

    for (i = 0; i<sizeOfMatrix; i = i+1)
        for (j = 0; j<sizeOfMatrix; j = j+1)
            if (i == j)
                Inverse[i][j] = 1;
            else
                Inverse[i][j] = 0;

	USART_Transmit('e');
	_delay_ms(100);
    for (k = 0; k<sizeOfMatrix; k = k+1)
    {
		
		_delay_ms(100);
		USART_Transmit('i');
		_delay_ms(100);
		app = input[k][k];
		lindex = k;


		for (uint8_t l = k + 1; l < sizeOfMatrix; l = l+1)
		if (fabs(app) < fabs(input[l][k])) {
			app = input[l][k];
			lindex = l;
		}

		USART_Transmit('j');
		_delay_ms(100);
		for (j = 0; j <= sizeOfMatrix; j = j+1)
		{
			temp[j] = input[lindex][j];
			input[lindex][j] = input[k][j];
			input[k][j] = temp[j];
		}

		for (j = 0; j <= sizeOfMatrix; j = j+1)
		{
			temp[j] = Inverse[lindex][j];
			Inverse[lindex][j] = Inverse[k][j];
			Inverse[k][j] = temp[j];
		}
		
		
		USART_Transmit('k');
		_delay_ms(100);
		
        localVariable = input[k][k];
        for (uint8_t j = 0; j<sizeOfMatrix; j = j+1)
        {
			USART_Transmit('q');
			USART_Transmit(j+48);
			_delay_ms(100);
            input[k][j] /= localVariable;
            Inverse[k][j] /= localVariable;
        }
		USART_Transmit('l');
		_delay_ms(100);
        for (uint8_t m = 0; m<sizeOfMatrix; m = m+1)
        {
			USART_Transmit('m');
			_delay_ms(100);
			USART_Transmit(m+48);
			_delay_ms(100);
			USART_Transmit(m+48);
			_delay_ms(100);
			USART_Transmit(m+48);
			_delay_ms(100);
            localVariable = input[m][k];
            for (uint8_t n= 0; n<sizeOfMatrix; n = n+1)
            {
				USART_Transmit('z');
				USART_Transmit(sizeOfMatrix+48);
				_delay_ms(100);
				USART_Transmit(m+48);
				USART_Transmit(n+48);
				USART_Transmit(k+48);
				USART_Transmit('z');
				_delay_ms(100);
                if (m == k){
					USART_Transmit('o');
                    break;
				}
				USART_Transmit('p');
				_delay_ms(100);
                input[m][n] -= input[k][n] * localVariable;
                Inverse[m][n] -= Inverse[k][n] * localVariable;
            }
        }
    }
	
	
	USART_Transmit('f');
	
	unsigned char a = (char)TCNT0;
	//unsigned char b = (char)TCNT1L;
	unsigned char over = (char)tot_overflow;
	
    //printf("The inverse matrix is:\n");
	
	
	//USART_TransmitF(Inverse[0][1]);
	USART_Transmit('w');
	_delay_ms(100);
	USART_Transmit('w');
	USART_Transmit('w');
	_delay_ms(100);
	USART_Transmit('w');
	
	while(1){
		/*
		USART_Transmit(a);
		USART_Transmit(over);
		USART_Transmit(over);
		USART_Transmit(over);
		USART_Transmit((char)tot_overflow);
		*/
		//USART_TransmitF(-0.07938568);
		
		float s = Inverse[1][1];
		USART_TransmitF(s);
		USART_Transmit('s');
		_delay_ms(100);
	}
	
	
    return 0;
}




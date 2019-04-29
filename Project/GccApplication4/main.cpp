/*
 * GccApplication4.cpp
 *
 * Created: 2019-04-13 2:19:35 PM
 * Author : Elite Phantom
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "SPI_MS.h"
#include "Font.h"
#include "myDS1307RTC.h"
void N5110_Cmnd(char DATA)
{
	PORTB &= ~(1<<DC);				/* make DC pin to logic zero for command operation */
	SPI_SS_Enable();				/* enable SS pin to slave selection */
	SPI_Write(DATA);				/* send data on data register */
	PORTB |= (1<<DC);				/* make DC pin to logic high for data operation */
	SPI_SS_Disable();
}

void N5110_Data(char *DATA)
{
	PORTB |= (1<<DC);									/* make DC pin to logic high for data operation */
	SPI_SS_Enable();									/* enable SS pin to slave selection */
	int lenan = strlen(DATA);							/* measure the length of data */
	for (int g=0; g<lenan; g++)
	{
		for (int index=0; index<5; index++)
		{
			SPI_Write(ASCII[DATA[g] - 0x20][index]);	/* send the data on data register */
		}
		SPI_Write(0x00);
	}
	SPI_SS_Disable();
}
void N5110_OnceData(char DATA)
{
	PORTB |= (1<<DC);									/* make DC pin to logic high for data operation */
	SPI_SS_Enable();									/* enable SS pin to slave selection */							/* measure the length of data */
	for (int index=0; index<5; index++)
	{
		SPI_Write(ASCII[DATA - 0x20][index]);	/* send the data on data register */
	}	/* send the data on data register */
		SPI_Write(0x00);
		SPI_SS_Disable();
}
void N5110_ReserveData(char *DATA)
{
	PORTB |= (1<<DC);									/* make DC pin to logic high for data operation */
	SPI_SS_Enable();									/* enable SS pin to slave selection */
	int lenan = strlen(DATA);							/* measure the length of data */
	for (int g=0; g<lenan; g++)
	{
		for (int index=0; index<5; index++)
		{
			SPI_Write(0xFF^(ASCII[DATA[g] - 0x20][index]));	/* send the data on data register */
		}
		SPI_Write(0xFF);
	}
	SPI_SS_Disable();
}

void N5110_Reset()					/* reset the Display at the beginning of initialization */
{
	PORTB &= ~(1<<RST);
	_delay_ms(100);
	PORTB |= (1<<RST);
}

void N5110_init()
{
	N5110_Reset();					/* reset the display */
	N5110_Cmnd(0x21);				/* command set in addition mode */
	N5110_Cmnd(0xC0);				/* set the voltage by sending C0 means VOP = 5V */
	N5110_Cmnd(0x07);				/* set the temp. coefficient to 3 */
	N5110_Cmnd(0x13);				/* set value of Voltage Bias System */
	N5110_Cmnd(0x20);				/* command set in basic mode */
	N5110_Cmnd(0x0C);				/* display result in normal mode */
}

void lcd_setXY(char x, char y)		/* set the column and row */
{
	N5110_Cmnd(x);
	N5110_Cmnd(y);
}

void N5110_clear()					/* clear the Display */
{
	SPI_SS_Enable();
	PORTB |= (1<<DC);
	for (int k=0; k<=503; k++)
	{
		SPI_Write(0x00);
	}
	PORTB &= ~(1<<DC);
	SPI_SS_Disable();
}
void PrintArray(char a[10][14], int x, int y)
{
	int z = 0;
	for (int i = 0; i < 5; i++)
	{
		if(z == 0 && y != 1)
		{
			lcd_setXY(0x41,0x80);
			N5110_Data(a[x]);
		}
		if(z == 0 && y == 1)
		{
			lcd_setXY(0x41,0x80);
			N5110_ReserveData(a[x]);
		}
		if(z == 1 && y != 2)
		{
			lcd_setXY(0x42,0x80);
			N5110_Data(a[x + 1]);
		}
		if(z == 1 && y == 2)
		{
			lcd_setXY(0x42,0x80);
			N5110_ReserveData(a[x + 1]);
		}
		if(z == 2 && y != 3)
		{
			lcd_setXY(0x43,0x80);
			N5110_Data(a[x + 2]);
		}
		if(z == 2 && y == 3)
		{
			lcd_setXY(0x43,0x80);
			N5110_ReserveData(a[x + 2]);
		}
		if(z == 3 && y != 4)
		{
			lcd_setXY(0x44,0x80);
			N5110_Data(a[x + 3]);
		}
		if(z == 3 && y == 4)
		{
			lcd_setXY(0x44,0x80);
			N5110_ReserveData(a[x + 3]);
		}
		if(z == 4 && y != 5)
		{
			lcd_setXY(0x45,0x80);
			N5110_Data(a[x + 4]);
		}
		if(z == 4 && y == 5)
		{
			lcd_setXY(0x45,0x80);
			N5110_ReserveData(a[x + 4]);
		}
		z++;
	}
}


volatile uint8_t	Second=55, Minute=59, Hour=10, Day=1, Date=31, Month=12, Year=18, Mode=1, AP=1;
volatile uint8_t tData[7], Time_count=0;
uint8_t length = 7;
char dis[5];

uint8_t BCD2Dec(uint8_t BCD){
	uint8_t L, H;
	L=BCD & 0x0F;
	H=(BCD>>4)*10;
	return (H+L);
}


uint8_t Dec2BCD(uint8_t Dec){
	uint8_t L, H;
	L=Dec % 10;
	H=(Dec/10)<<4;
	return (H+L);
}


char convertinttochar(uint8_t a)
{
	if(a == 0)
		return '0';
	if(a == 1)
		return '1';
	else if(a == 2)
		return '2';
	else if(a == 3)
		return '3';
	else if(a == 4)
		return '4';
	else if(a == 5)
		return '5';
	else if(a == 6)
		return '6';
	else if(a == 7)
		return '7';
	else if(a == 8)
		return '8';
	else if(a == 9)
		return '9';
}
 

void display()
{
		Second 	= BCD2Dec(tData[0] & 0x7F);
		Minute 	= BCD2Dec(tData[1]);
		
		if (Mode !=0) 	Hour = BCD2Dec(tData[2] & 0x1F); //mode 12h
		else 		  	Hour = BCD2Dec(tData[2] & 0x3F); //mode 24h
		
		Date   	= BCD2Dec(tData[4]);
		Month	= BCD2Dec(tData[5]);
		Year	= BCD2Dec(tData[6]);
		SPI_Init();
		N5110_init();
		lcd_setXY(0x40,0x80);
		N5110_OnceData(convertinttochar(Hour/10));
		lcd_setXY(0x40,0x86);
		N5110_OnceData(convertinttochar(Hour%10));
		lcd_setXY(0x40,0x92);
		N5110_OnceData(':');
		lcd_setXY(0x40,0x98);
		N5110_OnceData(convertinttochar(Minute/10));
		lcd_setXY(0x40,0x104);
		N5110_OnceData(convertinttochar(Minute%10));
		lcd_setXY(0x40,0x110);
		N5110_OnceData(':');
		lcd_setXY(0x40,0x116);
		N5110_OnceData(convertinttochar(Second/10));
		lcd_setXY(0x40,0x120);
		N5110_OnceData(convertinttochar(Second%10));
		lcd_setXY(0x41,0x80);
		N5110_OnceData(convertinttochar(Date/10));
		lcd_setXY(0x41,0x86);
		N5110_OnceData(convertinttochar(Date%10));
		lcd_setXY(0x41,0x92);
		N5110_OnceData('/');
		lcd_setXY(0x41,0x98);
		N5110_OnceData(convertinttochar(Month/10));
		lcd_setXY(0x41,0x104);
		N5110_OnceData(convertinttochar(Month%10));
		lcd_setXY(0x41,0x110);
		N5110_OnceData('/');
		lcd_setXY(0x41,0x116);
		N5110_Data("20");
		lcd_setXY(0x41,0x120);
		N5110_OnceData(convertinttochar(Year/10));
		lcd_setXY(0x41,0x120);
		N5110_OnceData(convertinttochar(Year%10));
}


int main(void)
{
	DDRD=0x00;
	PORTD=0xFF;
	MCUCR|=(1<<ISC11)|(1<<ISC10);
	GICR    |=(1<<INT1);
	sei();
    tData[0]=Dec2BCD(Second);
    tData[1]=Dec2BCD(Minute);
    if (Mode!=0) tData[2]=Dec2BCD(Hour)|(Mode<<6)|(AP<<5); //mode 12h
    else tData[2]=Dec2BCD(Hour);
    tData[3]=Dec2BCD(Day);
    tData[4]=Dec2BCD(Date);
    tData[5]=Dec2BCD(Month);
    tData[6]=Dec2BCD(Year);
    TWI_Init(); //khoi dong TWI
    TWI_DS1307_wblock(0x00, tData, length);
	_delay_ms(5);
	TWI_DS1307_wadr(0x00); 				//set dia chi ve 0
	_delay_ms(1);		   				//cho DS1307 xu li
    while (1) 
    {
    }
	return 0;
}
ISR(INT1_vect){
	TWI_DS1307_wadr(0x00); 				//set dia chi ve 0
	_delay_ms(1);		   				//cho DS1307 xu li
	TWI_DS1307_rblock(tData, length);
	display();
}


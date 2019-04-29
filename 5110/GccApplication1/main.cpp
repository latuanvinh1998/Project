#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <string.h>
#include "SPI_MS.h"
#include "Font.h"
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

int main()
{
	volatile uint8_t	Second=55, Minute=59, Hour=15, Day=1, Date=35, Month=12, Year=18, Mode=1, AP=1;
	while(1)
	{
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
		lcd_setXY(0x41,0x128);
		N5110_OnceData(convertinttochar(Year/10));
		lcd_setXY(0x41,0x134);
		N5110_OnceData(convertinttochar(Year%10));
	}
	return 0;
}
/*
 * CPPFile1.cpp
 *
 * Created: 5/27/2019 11:06:59 PM
 *  Author: Elite Phantom
 */ 
#include "Ram.h"

void enable()
{
	PORTB &= ~(1<<SS);
}


void disable()
{
	PORTB |= (1<<SS);
}


void Init()
{
	DDRB = (1<<MOSI)|(1<<SCK)|(1<<SS);				/* Set MOSI and SCK output, all others input */											/* define reset pin as a output */
	DDRB &= ~(1<<MISO);												/* define MISO pin as a input */
	PORTB |= (1<<SS);												/* Make SS pin as a output */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);							/* Enable SPE, MSTR and SPR0 bit in SPI control register */
}


char SPI_Read()
{
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));										/* wait here while SPIF flag set */
	return(SPDR);													/* Return the SPDR value */
}


void SPI_Write(char write_data)
{
	SPDR = write_data;
	while(!(SPSR & (1<<SPIF)));										/* wait here while SPIF flag set */
}


uint8_t ReadDataByte(uint16_t address)
{
	uint8_t data;
	enable();
	SPI_Write(READ);
	SPI_Write(address>>8);
	SPI_Write(address);
	data = SPI_Read();
	disable();
	return data;
}


void WriteDataByte(uint16_t address, uint8_t data)
{
	enable();
	SPI_Write(WRITE);
	SPI_Write(address>>8);
	SPI_Write(address);
	SPI_Write(data);
	disable();
}
/*
 * CPPFile1.cpp
 *
 * Created: 5/27/2019 11:06:59 PM
 *  Author: Elite Phantom
 */ 
#include "Ram.h"

uint8_t _current_mode;

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
	_current_mode = UNDEFINED_MODE;
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
	SetUp(BYTE_MODE, READ, address);
	data = SPI_Read();
	disable();
	return data;
}


void WriteDataByte(uint16_t address, char data)
{
	SetUp(BYTE_MODE, WRITE, address);
	SPI_Write(data);
	disable();
}

void ReadDataPage(uint16_t address, char *buffer)
{
	int i;
	SetUp(PAGE_MODE, READ, address);
	for (i = 0; i < 32; i++)
		buffer[i] = SPI_Read();
	disable();
}

void WriteDataPage(uint16_t address, char *buffer)
{
	int i;
	SetUp(PAGE_MODE, WRITE, address);
	for (i = 0; i < 32; i++)
		SPI_Write(buffer[i]);
	disable();
}

void ReadDataStream(uint16_t address, char *buffer)
{
	int i;
	SetUp(PAGE_MODE, WRITE, address);
	for (i = 0; i < 65536; i++)
		buffer[i] = SPI_Read();
	disable();
}

void WriteDataStream(uint16_t address, char *buffer)
{
	int i;
	SetUp(PAGE_MODE, WRITE, address);
	for (i = 0; i < 65536; i++)
		SPI_Write(buffer[i]);
	disable();
}

void SetMode(uint8_t mode)
{
	if(mode != _current_mode)
	{
		enable();
		SPI_Write(WRSR);
		SPI_Write(mode);
		disable();
		enable();
		SPI_Write(RDSR);
		_current_mode = SPI_Read();
		disable();
	}
}

void SetUp(uint8_t mode, uint8_t action, uint16_t address)
{
	SetMode(mode);
	enable();
	SPI_Write(action);
	SPI_Write(address>>8);
	SPI_Write(address);
}
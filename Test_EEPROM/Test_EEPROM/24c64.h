#ifndef _24C64_H_
#define _24C64_H_

#define FALSE 0
#define TRUE 1
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef cbi
#define cbi(port, bit) 	   (port) &= ~(1 << (bit))
#endif
#ifndef sbi
#define sbi(port, bit) 	   (port) |=  (1 << (bit))
#endif

#define _222K	10
#define _100K	32
#define TWI_W	0
#define TWI_R	1

#define TWI_START	(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
#define TWI_STOP	(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)
#define TWI_Clear_TWINT	(1<<TWINT)|(1<<TWEN)
#define TWI_Read_ACK	(1<<TWINT)|(1<<TWEA)|(1<<TWEN)

void EEOpen()
{
	TWSR=0x00; 
	TWBR= 32;
	TWCR=(1<<TWINT)|(1<<TWEN);
}
uint8_t EEWriteByte(uint16_t address,char data)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & 0x80)==0x00);
	
	if((TWSR & 0xF8) != 0x08)
		return TWSR;

	TWDR=0b10100000;	

	TWCR=TWI_Clear_TWINT; 

	while((TWCR & 0x80)==0x00);
		
	if((TWSR&0xF8) !=0x18)
		return TWSR;
	TWDR=(address>>8);

	TWCR=TWI_Clear_TWINT;
	
	while((TWCR & 0x80)==0x00);	
	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	TWDR=(address);

	TWCR=TWI_Clear_TWINT;
	
	while((TWCR & 0x80)==0x00);	

	if((TWSR & 0xF8) != 0x28)
		return TWSR;
		
	TWDR=(data);
	
	TWCR=(1<<TWINT)|(1<<TWEN);
	while((TWCR & 0x80)==0x00);	

	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(TWCR & (1<<TWSTO));
	
	_delay_ms(12);
	
	return 1;

}

char EEReadByte(uint16_t address)
{
	char data;
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

		while((TWCR & 0x80)==0x00);	

		if((TWSR & 0xF8) != 0x08)
			return TWSR;

		TWDR=0b10100000;	

		TWCR=(1<<TWINT)|(1<<TWEN);

		while((TWCR & 0x80)==0x00);	
		

	TWDR=(address>>8);

	TWCR=(1<<TWINT)|(1<<TWEN);

	while((TWCR & 0x80)==0x00);	

	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	TWDR=(address);

	TWCR=(1<<TWINT)|(1<<TWEN);

	while((TWCR & 0x80)==0x00);	

	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	while((TWCR & 0x80)==0x00);	

	if((TWSR & 0xF8) != 0x10)
		return TWSR;

	TWDR=0b10100001;	

	TWCR=(1<<TWINT)|(1<<TWEN);

	while((TWCR & 0x80)==0x00);	

	if((TWSR & 0xF8) != 0x40)
		return TWSR;

	TWCR=(1<<TWINT)|(1<<TWEN);

	while((TWCR & 0x80)==0x00);	

	if((TWSR & 0xF8) != 0x58)
		return TWSR;

	data=TWDR;

	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	while(TWCR & (1<<TWSTO));

	return data;
}


		



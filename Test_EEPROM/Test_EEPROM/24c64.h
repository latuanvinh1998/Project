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
	//Set up TWI Module
	TWSR=0x00; 
	TWBR= 32;
	TWCR=(1<<TWINT)|(1<<TWEN);
}
uint8_t EEWriteByte(uint16_t address,uint8_t data)
{
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & 0x80)==0x00);
	if((TWSR & 0xF8) != 0x08)
		return TWSR;

	//Now write SLA+W
	//EEPROM @ 00h
	TWDR=0b10100000;	

	//Initiate Transfer
	TWCR=TWI_Clear_TWINT; 

	//Poll Till Done
	while((TWCR & 0x80)==0x00);
		
	if((TWSR&0xF8) !=0x18)
		return TWSR;
	//Now write ADDRH
	TWDR=(address>>8);

	//Initiate Transfer
	TWCR=TWI_Clear_TWINT;
	
	while((TWCR & 0x80)==0x00);	

	//Check status
	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	//Now write ADDRL
	TWDR=(address);

	//Initiate Transfer
	TWCR=TWI_Clear_TWINT;
	
	while((TWCR & 0x80)==0x00);	

	//Check status
	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	//Now write DATA
	TWDR=(data);

	//Initiate Transfer
	TWCR=(1<<TWINT)|(1<<TWEN);

	//Poll Till Done
	while((TWCR & 0x80)==0x00);	

	//Check status
	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	//Put Stop Condition on bus
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	//Wait for STOP to finish
	while(TWCR & (1<<TWSTO));

	//Wait untill Writing is complete
	_delay_ms(12);

	//Return TRUE
	return 1;

}

uint8_t EEReadByte(uint16_t address)
{
	uint8_t data;

	//Initiate a Dummy Write Sequence to start Random Read
		//Put Start Condition on TWI Bus
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

		//Poll Till Done
		while((TWCR & 0x80)==0x00);	

		//Check status
		if((TWSR & 0xF8) != 0x08)
			return TWSR;

		//Now write SLA+W
		//EEPROM @ 00h
		TWDR=0b10100000;	

		//Initiate Transfer
		TWCR=(1<<TWINT)|(1<<TWEN);

		//Poll Till Done
		while((TWCR & 0x80)==0x00);	
		

	//Now write ADDRH
	TWDR=(address>>8);

	//Initiate Transfer
	TWCR=(1<<TWINT)|(1<<TWEN);

	//Poll Till Done
	while((TWCR & 0x80)==0x00);	

	//Check status
	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	//Now write ADDRL
	TWDR=(address);

	//Initiate Transfer
	TWCR=(1<<TWINT)|(1<<TWEN);

	//Poll Till Done
	while((TWCR & 0x80)==0x00);	

	//Check status
	if((TWSR & 0xF8) != 0x28)
		return TWSR;

	//*************************DUMMY WRITE SEQUENCE END **********************


	
	//Put Start Condition on TWI Bus
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	//Poll Till Done
	while((TWCR & 0x80)==0x00);	

	//Check status
	if((TWSR & 0xF8) != 0x10)
		return TWSR;

	//Now write SLA+R
	//EEPROM @ 00h
	TWDR=0b10100001;	

	//Initiate Transfer
	TWCR=(1<<TWINT)|(1<<TWEN);

	//Poll Till Done
	while((TWCR & 0x80)==0x00);	

	//Check status
	if((TWSR & 0xF8) != 0x40)
		return TWSR;

	//Now enable Reception of data by clearing TWINT
	TWCR=(1<<TWINT)|(1<<TWEN);

	//Wait till done
	while((TWCR & 0x80)==0x00);	

	//Check status
	if((TWSR & 0xF8) != 0x58)
		return TWSR;

	//Read the data
	data=TWDR;

	//Put Stop Condition on bus
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	//Wait for STOP to finish
	while(TWCR & (1<<TWSTO));

	//Return TRUE
	return data;
}


		



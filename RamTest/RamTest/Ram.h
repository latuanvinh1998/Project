/*
 * IncFile1.h
 *
 * Created: 5/27/2019 11:06:47 PM
 *  Author: Elite Phantom
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_


#include <avr/io.h>

#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2

#define MOSI 3
#define MISO 4
#define SCK 5
#define SS 2

#define BYTE_MODE (0x00 | HOLD)
#define PAGE_MODE (0x80 | HOLD)
#define STREAM_MODE (0x40 | HOLD)
#define RESERVED_MODE (0xC0 | HOLD)
#define UNDEFINED_MODE (0xFF)

void Enable();
void Disable();
void Init();
char SPI_Read();
void SPI_Write();
uint8_t ReadDataByte(uint16_t address);
void WriteDataByte(uint16_t address, uint8_t data);



#endif /* INCFILE1_H_ */
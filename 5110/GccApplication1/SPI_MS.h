/* 
* SPI_MS.h
*
* Created: 2019-04-06 11:32:40 PM
* Author: Elite Phantom
*/


#ifndef __SPI_MS_H__
#define __SPI_MS_H__


#include <avr/io.h>

#define MOSI 3
#define MISO 4
#define SCK 5
#define SS 2
#define DC 1
#define RST 0

void SPI_SS_Enable();
void SPI_SS_Disable();
void SPI_Init();
void SPI_Write(char write_data);
char SPI_Read();


#endif /* SPI_MASTER_H_FILE_H_ */

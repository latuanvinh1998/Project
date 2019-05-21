//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
// Author : Ho Thanh Tam	                                                         ;
// Email:   thanhtam.h@gmail.com													 ;		
// Date :   2009, Sep., 15                                                           ;
// Version: 0.1                                                                      ;
// Title:   TWI_Master                                                               ;
// website: www.hocavr.com                                                           ;
// Description: Dieu khien module TWI cua AVR, che do Master.                        ;
// - Dieu khien 1 ds1307 dong ho thoi gian thuc                                      ;
// - Goi/nhan cac goi data khac nhau den cac Slaves                                  ;
//                                                                                   ; 
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#include <avr/io.h>

#ifndef cbi
	#define cbi(port, bit) 	   (port) &= ~(1 << (bit))
#endif
#ifndef sbi
	#define sbi(port, bit) 	   (port) |=  (1 << (bit))
#endif

//dinh nghia cac duong giao tiep SPI tren AVR, phu thuoc cau truc chan cua tung chip
#define TWI_PORT	PORTC
#define TWI_DDR		DDRC
#define	TWI_PIN		PINC

#define	SDA_PIN		4
#define	SCL_PIN		5
//------------------------------

#define	DS1307_SLA	0X68 //dia chi I2C SLA mac dinh cua DS1307: 0x68=1101000

//--dinh nghia gia tri cho thanh toc do TWBR ung voi tan so xung giu nhip 8MHz
#define _222K	10
#define _100K	32

//--command for TWCR--------------
#define TWI_W	0	// bit Write
#define TWI_R	1	// bit Read

#define TWI_START	(1<<TWINT)|(1<<TWSTA)|(1<<TWEN)	//0xA4: goi Start condidition
#define TWI_STOP	(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)	//0x94: goi STOP condition

#define TWI_Clear_TWINT	(1<<TWINT)|(1<<TWEN)		    //0x84 : xoa TWIN de bat dau doc, doc xong thi NOT ACK
#define TWI_Read_ACK	(1<<TWINT)|(1<<TWEN)|(1<<TWEA)	//0xC4 : xoa TWIN de bat dau doc, sau khi doc set ACK

//Khoi dong TWI
void TWI_Init(void){
	TWSR=0x00; //Prescaler=1
	TWBR=_100K;
	TWCR=(1<<TWINT)|(1<<TWEN);
}


uint8_t TWI_DS1307_wadr(uint8_t Addr){ 
		
	TWCR=TWI_START;						
	while((TWCR & 0x80)==0x00);		 
	if((TWSR&0xF8) !=0x08) return 1;
	
	TWDR=(DS1307_SLA<<1)+TWI_W; 	
	TWCR=TWI_Clear_TWINT; 				 
	while((TWCR & 0x80)==0x00);		
	if((TWSR&0xF8) !=0x18) return 1; 
		
	TWDR=Addr; 							 
	TWCR=TWI_Clear_TWINT; 				
	while((TWCR & 0x80)==0x00);		 
	if((TWSR&0xF8) !=0x28) return 1;
	
	TWCR=TWI_STOP;					  
	return 0;
}

uint8_t TWI_DS1307_wblock(volatile uint8_t Addr,volatile uint8_t Data[], uint8_t len){
		
	TWCR=TWI_START; 					 //goi START condition
	while((TWCR & 0x80)==0x00);		 //cho TWINT bit=1
	if((TWSR&0xF8) !=0x08) return 1; //neu goi Start co loi thi thoat
	
	TWDR=(DS1307_SLA<<1)+TWI_W; 		 //dia chi DS va bit W 	
	TWCR=TWI_Clear_TWINT; 				 //xoa TWINT de bat dau goi
	while((TWCR & 0x80)==0x00);		 //cho TWINT bit=1
	if((TWSR&0xF8) !=0x18) return 1; //neu co loi truyen SLA, thoat
	
	TWDR=Addr; 							 //goi dia chi thanh ghi can ghi vao
	TWCR=TWI_Clear_TWINT; 				 //xoa TWINT de bat dau goi
	while((TWCR & 0x80)==0x00);		 //cho TWINT bit=1
	if((TWSR&0xF8) !=0x28) return 1; 

	for (uint8_t i=0; i <len; i++){
		TWDR=Data[i]; 						 //chuan bi xuat du lieu
		TWCR=TWI_Clear_TWINT;  				 //xoa TWINT, bat dau send
		while((TWCR & 0x80)==0x00);	 	 //cho TWINT bit=1
		if((TWSR&0xF8) !=0x28) return 1; //neu status ko phai la 0x28 thi return
	}
	
	TWCR=TWI_STOP;							 //STOP condition
	return 0;
}

//doc 1 mang tu DS
uint8_t TWI_DS1307_rblock(volatile uint8_t Data[], uint8_t len ){ 	
	uint8_t i;
	TWCR=TWI_START; // Start--------------------------------------------------------------------
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x08));	 //cho TWINT bit=1 va goi START thanh cong
	TWDR=(DS1307_SLA<<1)+TWI_R; 							 //goi dia chi SLA +READ	
	TWCR=TWI_Clear_TWINT; 									 //bat dau, xoa TWINT
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x40));	 //cho TWINT bit=1	va goi SLA thanh cong
	//nhan len-1 bytes dau tien---------------------
    for (i=0; i <len-1; i++){
       TWCR=TWI_Read_ACK; 									 //xoa TWINT,se goi ACK sau khi nhan moi byte
       while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x50));//cho TWINT bit=1 hoac nhan duoc ACK	   
	   Data[i]=TWDR;										  //doc du lieu vao mang Data 
    }
	//nhan byte cuoi
	TWCR=TWI_Clear_TWINT; 									 //xoa TWINT de nhan byte cuoi, sau do set NOT ACK
	while (((TWCR & 0x80)==0x00)||((TWSR&0xF8) !=0x58));   //cho TWIN=1 hoac trang thai not ack	
	Data[len-1]=TWDR;
	
	TWCR=TWI_STOP;											 //STOP condition
	return 0;	
}
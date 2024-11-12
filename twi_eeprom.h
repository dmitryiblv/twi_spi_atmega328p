/*
Regsiters
	https://eu.mouser.com/datasheet/2/268/doc8008-1315488.pdf -> page 156
	TWBR    TWI Bit Rate Register
                                        init
            TWBR7-TWBR0     Bits 7-0    0   R/W     TWI Bit Rate Register
           
	TWCR	TWI Control Register                           
			TWINT	Bit 7	0   R/W     TWI Interrupt Flag
			TWEA	Bit 6	0   R/W     TWI Enable Acknowledge Bit
			TWSTA	Bit 5	0   R/W     TWI START Condition Bit
			TWSTO	Bit 4	0   R/W     TWI STOP Condition Bit
			TWWC	Bit 3	0   R/W     TWI Write Collision Flag
			TWEN	Bit 2	0   R/W     TWI Enable Bit
			-		Bit 1	-   R       Reserved Bit
			TWIE	Bit 0	0   R/W     TWI Interrupt Enable
		
	TWSR    TWI Status Register	     
			TWS7-TWS3       Bits 7-3    1   R       TWI Status
			-               Bit 2		-   R       Reserved Bit
			TWPS1-TWPS0     Bits 1-0 	0   R/W     TWI Prescaler Bits
	
	TWDR	TWI Data Register			
			TWD7-TWD0   Bits 7-0	1   R/W     TWI Data Register
			
	TWAR	TWI (Slave) Address Register
            TWA6-TWA0   Bits 7-1    1   R/W     TWI (Slave) Address Register 
            TWGCE       Bit 0       0   R/W     TWI General Call Recognition Enable Bit 
            
    TWAMR   TWI (Slave) Address Mask Register
            TWAM6-TWAM0     Bits 7-1    0   R/W     TWI Address Mask
            -               Bit 0       -   R       Reserved Bit
*/
#ifndef TWI_EEPROM_H
#define TWI_EEPROM_H

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "init.h"

#define TWI_FREQ 400000LU // 400 KHz

#define TWI_BUS_DELAY_USEC 1.3 // microsec; Bus must be free for at least this time
#define TWI_BUS_DELAY_RESET_USEC 5 // microsec; Bus delay during reset

// https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf -> page 198
// General TWI Master status codes                      
#define TWI_STATUS_START                0x08  // START has been transmitted
#define TWI_STATUS_START_REP            0x10  // Repeated START has been transmitted
#define TWI_STATUS_ARB_LOST             0x38  // Arbitration lost
// TWI Master Transmitter status codes
#define TWI_STATUS_W_ADDR_ACK           0x18  // SLA+W has been transmitted and ACK received
#define TWI_STATUS_W_ADDR_NACK          0x20  // SLA+W has been transmitted and NACK received 
#define TWI_STATUS_W_DATA_ACK           0x28  // Data byte has been transmitted and ACK received
#define TWI_STATUS_W_DATA_NACK          0x30  // Data byte has been transmitted and NACK received 
// TWI Master Receiver status codes  
#define TWI_STATUS_R_ADDR_ACK           0x40  // SLA+R has been transmitted and ACK received
#define TWI_STATUS_R_ADDR_NACK          0x48  // SLA+R has been transmitted and NACK received
#define TWI_STATUS_R_DATA_ACK           0x50  // Data byte has been received and ACK transmitted
#define TWI_STATUS_R_DATA_NACK          0x58  // Data byte has been received and NACK transmitted
// TWI Slave
// ... not used
// TWI Miscellaneous status codes
#define TWI_STATUS_NO_STATE             0xF8  // No relevant state information available; TWINT = ?0?
#define TWI_STATUS_BUS_ERR              0x00  // Bus error due to an illegal START or STOP condition

void TWI_init(void);

void TWI_start(void); 

void TWI_stop(void);

void TWI_write(uint8_t byte);

// Return error code
uint8_t TWI_readNack(void);

// devAddrWrite - Device address + tail bit is write
// memCellAddr - Address of a memory cell in EEPROM
// Return error code
uint8_t TWI_EEPROM_writeByte(uint8_t devAddrWrite, uint8_t memCellAddr, uint8_t byte);

// devAddrWrite - Device address + tail bit is write
// devAddrRead - Device address + tail bit is read
// memCellAddr - Memory cell address
uint8_t TWI_EEPROM_readByte(uint8_t devAddrWrite, uint8_t devAddrRead, uint8_t memCellAddr, uint8_t* byte);

uint8_t TWI_ping(uint8_t devAddrRead, bool* state);

void TWI_reset(void);

#endif
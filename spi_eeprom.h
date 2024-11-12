/*
Registers
    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf -> page 140
    SPCR    SPI Control Register                        
                                    init
            SPIE        Bit 7       0   R/W     SPI Interrupt Enable
            SPE         Bit 6       0   R/W     SPI Enable
            DORD        Bit 5       0   R/W     Data Order; 0 - MSB of the data word is transmitted first
            MSTR        Bit 4       0   R/W     Master/Slave Select; 0 - Slave mode, 1 - master mode
            CPOL        Bit 3       0   R/W     Clock Polarity; 0 - SCK is low when idle, Leading Edge: Rising, Trailing Edge: Falling
            CPHA        Bit 2       0   R/W     Clock Phase; Determine if data is sampled on the leading (first) or trailing (last) edge of SCK
                                                0 - Leading Edge: Sample, Trailing Edge: Setup
            SPR1, SPR0  Bits 1, 0   0   R/W     SPI Clock Rate Select 1 and 0

    SPSR    SPI Status Register
            SPIF    Bit 7       0   R       SPI Interrupt Flag
            WCOL    Bit 6       0   R       Write COLlision Flag
            -       Bit 5-1     0   R       Reserved Bits
            SPI2X   Bit 0       0   R/W     Double SPI Speed Bit
    
    SPDR    SPI Data Register
            -   Bit 7-0     undef   R/W     Data register; 7 - MSB, 0 - LSB    
*/
#ifndef SPI_EEPROM_H
#define SPI_EEPROM_H

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "init.h"

#define SPI_SLAVE_SELECT() PORTB &= ~(1 << DDB2)    // Set PB2/SS low
#define SPI_SLAVE_UNSELECT() PORTB |= (1 << DDB2)   // Set PB2/SS high

#define SPI_EEPROM_WRITE_DELAY_STEP_MS 75    // Delay step before check EEPROM write operation is complete

// Instructions
// http://ww1.microchip.com/downloads/en/DeviceDoc/21832H.pdf -> page 7
#define SPI_INSTR_READ          0x03 // 0000 x011 Read data from memory array beginning at selected address
#define SPI_INSTR_WRITE         0x02 // 0000 x010 Write data to memory array beginning at selected address
#define SPI_INSTR_W_DISABLE     0x04 // 0000 x100 Reset the write enable latch (disable write operations)
#define SPI_INSTR_W_ENABLE      0x06 // 0000 x110 Set the write enable latch (enable write operations)
#define SPI_INSTR_R_STATUS      0x05 // 0000 x101 Read STATUS register
#define SPI_INSTR_W_STATUS      0x01 // 0000 x001 Write STATUS register 

void SPI_masterInit(void);

// byte - Data to transmit
// Return error code
uint8_t SPI_masterTransmit(uint8_t byte);

// byte - Received data
// Return error code
uint8_t SPI_masterReceive(uint8_t* byte);

// status - Read status
// Return error code
uint8_t SPI_EEPROM_readStatus(uint8_t* status);

// memCellAddr - Address of a memory cell in EEPROM
// byte - Read data
// Return error code
uint8_t SPI_EEPROM_readByte(uint8_t memCellAddr, uint8_t* byte);

// memCellAddr - Address of a memory cell in EEPROM
// byte - Write data
// Return error code
uint8_t SPI_EEPROM_writeByte(uint8_t memCellAddr, uint8_t byte);

#endif
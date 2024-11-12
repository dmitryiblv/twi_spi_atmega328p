//
// ATmega328P - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
//      NOTE: Default int is 16 bit
// EEPROM I2C 24AA01 - http://ww1.microchip.com/downloads/en/devicedoc/21711j.pdf
// EEPROM SPI 25AA010A - http://ww1.microchip.com/downloads/en/DeviceDoc/21832H.pdf
//
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "init.h"
#include "usart.h"
#include "twi_eeprom.h"
#include "spi_eeprom.h"

long int _d, _dd; // Dev only

int main()
{     
    extern FILE myStdIo;
	stdin = stdout = &myStdIo;	
	USART_init();
    
    // SPI
    
    if (0) {
        SPI_masterInit();        
        SPI_EEPROM_writeByte(0x00, 0xA5);    
        
        uint8_t byte, err;
        err = SPI_EEPROM_readByte(0x00, &byte);
        printf("Read: %X\r\n", byte);
        printf("Err: %X\r\n", err);
        
        return 0;
    }
    
    // TWI

	uint8_t memCellAddr, keyId, err;
	
    TWI_init();
    
    // Ping
    if (0) {
        bool state;
        TWI_ping(KEY_EEPROM_ADDR_W, &state);
        _d = (int) state;
    }
    
    // Write
	if (0) {
		// id	addr		val
		// 1	0x00		0x01
		// 2	0x01		0x02
		// 3	0x02		0x03
		// 4	0x03		0x04
		// 5	0x04		0x05
		// 6	0x05		0x06
		// 7	0x06		0x07
		// 8	0x07		0x08
		// 9	0x08		0x09
		// 10	0x09		0x0A
		memCellAddr = 0x09;
		keyId = 0x0A;
		err = TWI_EEPROM_writeByte(KEY_EEPROM_ADDR_W, memCellAddr, keyId);
		printf("Write\r\n");
		printf("Err: %d\r\n", err);
	}
    
    // Read one
	if (1) {
		memCellAddr = 0x00;
		err = TWI_EEPROM_readByte(KEY_EEPROM_ADDR_W, KEY_EEPROM_ADDR_R, memCellAddr, &keyId);
		printf("Read: 0x%02X\r\n", keyId);
		printf("Err: %d\r\n", err);
        _d = keyId;
                
        TWI_reset();
        _d = 0xFF;
        err = TWI_EEPROM_readByte(KEY_EEPROM_ADDR_W, KEY_EEPROM_ADDR_R, memCellAddr, &keyId);
        _d = keyId;
	}    
    
    // Read all once
	if (1) {
		uint8_t memCellAddrs[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};		
		uint8_t i;
		uint8_t size = sizeof(memCellAddrs);
		for (i = 0; i < size; i++) {
			printf("Mem %d\r\n", i + 1);
			err = TWI_EEPROM_readByte(KEY_EEPROM_ADDR_W, KEY_EEPROM_ADDR_R, memCellAddrs[i], &keyId);
			printf("  read: 0x%X\r\n", keyId);
			printf("  err: %d\r\n", err);			
		}
	} 
    
    // Read all loop
	if (1) {
		uint8_t memCellAddrs[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};				
        uint8_t memVals[] = {0x01, 0x02, 0xFF, 0xFF, 0x05, 0xFF, 0x07, 0xFF, 0x09, 0xFF};
		uint8_t i;
		uint8_t size = sizeof(memCellAddrs);
		uint32_t loop = 1;
		while (1) {				
			for (i = 0; i < size; i++) {
				err = TWI_EEPROM_readByte(KEY_EEPROM_ADDR_W, KEY_EEPROM_ADDR_R, memCellAddrs[i], &keyId);				
				if (err || keyId != memVals[i]) {
					printf("Error: got: %X, expected: %X,\r\n  err: %d, addr: %X, loop: %lu\r\n", keyId, memVals[i], err, memCellAddrs[i], (unsigned long int) loop);				
					return 1;
				}		
			}
			if (!(loop % 10)) {
				printf("%lu\r\n", (unsigned long int) loop);
			}		
			loop++;
		}
	}
    
    return 0;
}
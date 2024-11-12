#include "twi.h"

void TWI_init(void) {
    // Set registers to default values
    TWCR = 0;
    TWSR = 0;
    TWDR = 0xFF;
    TWAR = 0; // At docs is 11111110, but at Proteus is 0x00
    TWAMR = 0;
    // Set bit rate register
    // https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf -> page 180
    TWBR = (CPU_FREQ / TWI_FREQ - 16) / 2; // 0x02    
}

uint8_t TWI_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while ( !(TWCR & (1 << TWINT)) ); // Wait for Interrupt Flag is set
                                      // OR: while (bit_is_clear(TWCR, TWINT));  
    return SUCCESS;
} 

void TWI_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_write(uint8_t byte) {
    TWDR = byte;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ( !(TWCR & (1 << TWINT)) );
}

uint8_t TWI_readNack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while ( !(TWCR & (1 << TWINT)) );
    return TWDR;
}

uint8_t TWI_EEPROM_writeByte(uint8_t devAddrWrite, uint8_t memCellAddr, uint8_t byte) {
    if (TWI_start()) {
        return ERROR(1);
    }
    if (TWSR != TWI_STATUS_START) {
        return ERROR(1);
    }

    TWI_write(devAddrWrite);
    if (TWSR != TWI_STATUS_W_ADDR_ACK) { 
        return ERROR(2); 
    }

    TWI_write(memCellAddr);
    if (TWSR != TWI_STATUS_W_DATA_ACK) { 
        return ERROR(3); 
    }

    TWI_write(byte);
    if (TWSR != TWI_STATUS_W_DATA_ACK) { 
        return ERROR(4); 
    }

    TWI_stop();

    _delay_us(TWI_BUS_DELAY_USEC);

    return SUCCESS;
}

uint8_t TWI_EEPROM_readByte(uint8_t devAddrWrite, uint8_t devAddrRead, uint8_t memCellAddr, uint8_t* byte) {
    if (TWI_start()) {
        return ERROR(1);
    }
    if (TWSR != TWI_STATUS_START) { 
        return ERROR(1); 
    }

    TWI_write(devAddrWrite);
    if (TWSR != TWI_STATUS_W_ADDR_ACK) { 
        return ERROR(2); 
    }

    TWI_write(memCellAddr);
    if (TWSR != TWI_STATUS_W_DATA_ACK) { 
        return ERROR(3); 
    }

    if (TWI_start()) {  // Start repeat
        return ERROR(1);
    }
    if (TWSR != TWI_STATUS_START_REP) { 
        return ERROR(4); 
    }

    TWI_write(devAddrRead);
    if (TWSR != TWI_STATUS_R_ADDR_ACK) { 
        return ERROR(5); 
    }

    *byte = TWI_readNack();
    if (TWSR != TWI_STATUS_R_DATA_NACK) { 
        return ERROR(6); 
    }

    TWI_stop();

    _delay_us(TWI_BUS_DELAY_USEC);

    return SUCCESS;
}

uint8_t TWI_ping(uint8_t devAddrRead, bool* state) {
    *state = false;

    if (TWI_start()) {
        return ERROR(1);
    }
    if (TWSR != TWI_STATUS_START) { 
        return ERROR(1);
    }

    TWI_write(devAddrRead);
    if (TWSR != TWI_STATUS_W_ADDR_ACK) { 
        return ERROR(2);
    }
    
    TWI_stop();

    _delay_us(TWI_BUS_DELAY_USEC);

    *state = true;
    
    return SUCCESS;
}

void TWI_reset(void) {
    _delay_us(TWI_BUS_DELAY_RESET_USEC);
    
    TWI_stop(); // Close transmission if exists        
    
    _delay_us(TWI_BUS_DELAY_RESET_USEC);
    
    TWI_init(); // Re-init TWI
}

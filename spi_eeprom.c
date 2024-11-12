#include "spi_eeprom.h"

void SPI_masterInit(void) {
    DDRB |= (1 << DDB2)     // Set output PB4/SS
        | (1 << DDB3)       // Set output PB5/MOSI
        | (1 << DDB5);      // Set output PB7/SCK
    DDRB &= ~(1 << DDB4);   // Set input PB6/MISO
    
    SPCR |= (1 << SPE)  // Enable SPI
        | (1 << MSTR);  // Master mode               
    // Clock rate: SPR1 is 0, SPR0 is 0 -> CLK_FREQ / 4 -> 2 MHz
    
    SPI_SLAVE_UNSELECT(); // Set SS port high    
}

uint8_t SPI_masterTransmit(uint8_t byte) {
    SPDR = byte;    
    while ( !(SPSR & (1 << SPIF)) ); // Wait for transmission complete
    return SUCCESS;
}

uint8_t SPI_masterReceive(uint8_t* byte) {
    SPDR = 0xFF; // To start reception we need to begin sending impulses on SCK, so transfer over SPI any byte    
    while ( !(SPSR & (1 << SPIF)) ); // Wait for reception complete    
    *byte = SPDR;    
    return SUCCESS;
}

uint8_t SPI_EEPROM_readStatus(uint8_t* status) {
    // Read status register - http://ww1.microchip.com/downloads/en/DeviceDoc/21832H.pdf -> page 10        
    SPI_SLAVE_SELECT();
    if (SPI_masterTransmit(SPI_INSTR_R_STATUS)) {
        return ERROR(1);
    }
    if (SPI_masterReceive(status)) {
        return ERROR(2);
    }    
    SPI_SLAVE_UNSELECT();   
    
    return SUCCESS;
}

uint8_t SPI_EEPROM_readByte(uint8_t memCellAddr, uint8_t* byte) {
    // Read sequence - http://ww1.microchip.com/downloads/en/DeviceDoc/21832H.pdf -> page 7        
    SPI_SLAVE_SELECT();
    if (SPI_masterTransmit(SPI_INSTR_READ)) {
        return ERROR(1);
    }    
    if (SPI_masterTransmit(memCellAddr)) {
        return ERROR(2);
    }    
    if (SPI_masterReceive(byte)) {
        return ERROR(3);
    }
    SPI_SLAVE_UNSELECT();
    
    return SUCCESS;
}

uint8_t SPI_EEPROM_writeByte(uint8_t memCellAddr, uint8_t byte) {
    // Write sequence - http://ww1.microchip.com/downloads/en/DeviceDoc/21832H.pdf -> page 6, 8 
    
    // Write enable
    SPI_SLAVE_SELECT();    
    if (SPI_masterTransmit(SPI_INSTR_W_ENABLE)) {
        return ERROR(1);
    }
    SPI_SLAVE_UNSELECT();
    
    // Write data
    SPI_SLAVE_SELECT(); 
    if (SPI_masterTransmit(SPI_INSTR_WRITE)) {
        return ERROR(2);
    }
    if (SPI_masterTransmit(memCellAddr)) {
        return ERROR(3);
    }
    if (SPI_masterTransmit(byte)) {
        return ERROR(4);
    }
    SPI_SLAVE_UNSELECT();    
    
    // Wait till write operation complete at EEPROM
    uint8_t status;
    for (;;) {
        _delay_ms(SPI_EEPROM_WRITE_DELAY_STEP_MS);        
        if (SPI_EEPROM_readStatus(&status)) {
            return ERROR(5);
        }
        if ( !(status & (1 << 0)) ) { // EEPROM status WIP bit is 0 if EEPROM is not busy with write operation
            break;
        }        
    }    
    return SUCCESS;    
}

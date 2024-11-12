#include "usart.h"

void USART_init() {
	uint16_t ubrrBaud = CLK_FREQ / (BAUD_RATE * 16) - 1; // 9615 b/s
    
    UBRR0H = (ubrrBaud >> 8); // 0x00; Write 4 right bits of 12 (8 + 4) of ubrrBaud
    UBRR0L = (uint8_t) ubrrBaud; // 0x33; Rate: 9615 b/s
    
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Set 8 bit Character Size    
    UCSR0B &= ~(1 << UCSZ02); // Set 8 bit Character Size (combined with UCSZ01, UCSZ00)    
    UCSR0B |= (1 << TXEN0); // Enable transmitter    
}

int USART_putchar(char symbol, FILE* stream) {
    while ( !(UCSR0A & (1 << UDRE0)) ); // OR: loop_until_bit_is_set(UCSR0A, UDRE0);    
    UDR0 = symbol;
	return 0;
}

int USART_getchar(FILE* stream) {
    while ( !(UCSR0A & (1 << RXC0)) );    
	return UDR0;
}

FILE myStdIo = FDEV_SETUP_STREAM(USART_putchar, USART_getchar, _FDEV_SETUP_RW);

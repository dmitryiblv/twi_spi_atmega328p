/*
Registers
    https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf -> page 159

    UDR0    USART I/O Data Register 0

    UCSR0A  USART Control and Status Register 0 A

    UCSR0B  USART Control and Status Register 0 B

    UCSR0C  USART Control and Status Register 0 C

    UBRR0L, UBRR0H  USART Baud Rate Registers

*/
#ifndef USART_H
#define USART_H

#include <stdio.h>
#include <avr/io.h>
#include "init.h"

#define BAUD_RATE 9600LU

void USART_init();

int USART_putchar(char symbol, FILE* stream);

int USART_getchar(FILE* stream);

#endif
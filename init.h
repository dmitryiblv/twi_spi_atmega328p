#ifndef INIT_H
#define INIT_H

#define SUCCESS 0
#define ERROR(code) code

#define CLK_FREQ 8000000LU // 8 MHz; Default internal oscillator
#define CPU_FREQ 8000000LU // 8 MHz; CLKDIV8 is off

// EEPROM address: 7 bits (4 bits hardware code (1010) + 3 bits hardware A0-A2 pins (000)) + tail bit write(0) or read(1)
#define KEY_EEPROM_ADDR_W 0xA0 // 0b10100000, addr + write
#define KEY_EEPROM_ADDR_R 0xA1 // 0b10100001, addr + read

#endif
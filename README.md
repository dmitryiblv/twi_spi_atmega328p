<pre>
SPI and TWI Interface for ATmega328P
  
Compile source to hex
    sudo apt install gcc-avr
    sudo apt install avr-libc

    avr-gcc -Wall -gdwarf-2 -fsigned-char -MD -MP -DF_CPU=1000000 -Os -mmcu=atmega328p -o main.o -c main.c
    avr-gcc -mmcu=atmega328p -o main.elf main.o
    avr-objcopy -O ihex -R .eeprom main.elf main.hex
</pre>

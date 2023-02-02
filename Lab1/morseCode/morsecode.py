import os

os.system('avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o morseCode.o morseCode.c')
os.system('avr-gcc -mmcu=atmega328p morseCode.o -o morseCode')
os.system('avr-objcopy -O ihex -R .eeprom morseCode morseCode.hex')
os.system('avrdude -patmega328p -Pcom24 -carduino -D -U flash:w:morseCode.hex:i')

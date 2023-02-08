import os
os.system('avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o stop_light.o stop_light.c')

os.system('avr-gcc -mmcu=atmega328p stop_light.o -o stop_light')

os.system('avr-objcopy -O ihex -R .eeprom stop_light stop_light.hex')

#os.system('avrdude -patmega328p -Pcom24 -carduino -D -U flash:w:stop_light.hex:i')

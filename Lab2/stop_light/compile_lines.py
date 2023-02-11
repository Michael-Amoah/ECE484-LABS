import os
import sys

if len(sys.argv) != 3:
    sys.exit("python compile_lines.py <path_to_c_file> <comXX>")

file_name = os.path.splitext(sys.argv[1])[0]
com_output = sys.argv[2]

os.system(f'avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o {file_name}.o {file_name}.c')

os.system(f'avr-gcc -mmcu=atmega328p {file_name}.o -o {file_name}')

os.system(f'avr-objcopy -O ihex -R .eeprom {file_name} {file_name}.hex')

os.system(f'avrdude -patmega328p -P{com_output} -carduino -D -U flash:w:{file_name}.hex:i')

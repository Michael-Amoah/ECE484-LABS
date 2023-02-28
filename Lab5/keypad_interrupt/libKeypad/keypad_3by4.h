#ifndef keypad_H_INCLUDED
#define keypad_H_INCLUDED

#include <inttypes.h>

void config_row_as(uint8_t direction);
void config_column_as(uint8_t direction);
void output_for_row_as(uint8_t value);
void output_on_1_for_row_as(uint8_t pin);
void output_for_column_as(uint8_t value);
uint8_t read_columns();
uint8_t read_rows();
void keypad_setup();
uint8_t key_pressed();

#endif

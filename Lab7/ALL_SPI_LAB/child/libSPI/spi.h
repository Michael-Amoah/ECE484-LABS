#ifndef SPI_H_INCLUDE
#define SPI_H_INCLUDE

//----- Headers ------------//
#include <inttypes.h>
#include <avr/io.h>
#include "IO_Macros.h"
#include "spi_settings.h"
//--------------------------//

//----- Prototypes ------------------------------------------------------------//
void SPI_parent_init();
uint8_t SPI_txrx_parent(uint8_t data_to_tx);
void SPI_init_child();
uint8_t SPI_txrx_child(uint8_t data_to_tx);
//-----------------------------------------------------------------------------//
#endif

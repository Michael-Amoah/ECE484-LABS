#include <util/delay.h>
#include "spi_settings.h"
#include "spi.h"

void spi_my_delay_ms(unsigned int delay) ;

void SPI_init_child()
{
#ifdef HW_SPI_CHILD
	uint8_t clean_reg;
#endif

	// set MISO 
	PinMode(SPI_MISO, Output);		
	PinMode(SPI_MOSI, Input);		
	PinMode(SPI_SCK, Input);		
	PinMode(SPI_CS, Input);		

	DigitalWrite(SPI_CS, High); // set to pullup - improves quality


	// enable SPI 
	// 1/fosc128
	// Interrupts
#ifdef HW_SPI_CHILD
#ifdef INTERRUPT_SPI_CHILD
	//SPCR = (1 << SPE) | (1 << SPR1) | (1 << SPR0);
	//SPCR = (1 << SPE) | (1 << SPR1); //fosc/64 
	SPCR = (1 << SPE) | (1 << SPR0); //fosc/16 

#else
	// No Interupts
	SPCR = (1 << SPE) | (1 << SPR1) | (1 << SPR0);
#endif

	clean_reg = SPSR;
	clean_reg = SPDR;
#endif
}

void SPI_parent_init()
{
#ifdef HW_SPI_PARENT
	uint8_t clean_reg;
#endif

	// Make sure it's high
	DigitalWrite(SPI_CS, High);

	// set CS, SPI_MOSI and SPI_SCK to output
	PinMode(SPI_CS, Output);		
	PinMode(SPI_MOSI, Output);		
	PinMode(SPI_SCK, Output);		
	PinMode(SPI_MISO, Input);		

	DigitalWrite(SPI_CS, High);
	DigitalWrite(SPI_SCK, Low);

	// enable SPI, set as master, and clock to fosc/128 - slow as possible
	// fosc/128
#ifdef HW_SPI_PARENT
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);

	clean_reg = SPSR;
	clean_reg = SPDR;
#endif
}

uint8_t SPI_txrx_child(uint8_t data_to_tx)
{
#ifdef HW_SPI_CHILD
	// load data into register
	SPDR = data_to_tx;

	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));

	return SPDR;
#else
	/* ASSUME SS is set LOW by software */
	uint8_t send = data_to_tx;
	uint8_t receive = 0x00;
	uint8_t bit_idx = 0x80;
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		if (send & bit_idx)
			DigitalWrite(SPI_MISO, High);
		else
			DigitalWrite(SPI_MISO, Low);
		/* wait to sample on high */
		while (DigitalRead(SPI_SCK) == Low);
		
		/* Sample on HIGH clk */
		if (DigitalRead(SPI_MOSI) == High)
			receive |= bit_idx;

		/* wait until clock goes low */
		while (DigitalRead(SPI_SCK) == High);

		bit_idx = bit_idx >> 1;
	}

	return receive;	
#endif
}

uint8_t SPI_txrx_parent(uint8_t data_to_tx)
{
	int tx_delay = 10;
#ifdef HW_SPI_PARENT
	// load data into register
	SPDR = data_to_tx;

	// Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));

	spi_my_delay_ms(tx_delay);

	return SPDR;
#else
	/* ASSUME SS is set LOW by software */
	uint8_t send = data_to_tx;
	uint8_t receive = 0x00;
	uint8_t bit_idx = 0x80;
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		if (send & bit_idx)
			DigitalWrite(SPI_MOSI, High);
		else
			DigitalWrite(SPI_MOSI, Low);
		spi_my_delay_ms(2);	
		DigitalWrite(SPI_SCK, High);
		spi_my_delay_ms(2);
		if (DigitalRead(SPI_MISO) == High)
			receive |= bit_idx;
		DigitalWrite(SPI_SCK, Low);
		bit_idx = bit_idx >> 1;
	}

	spi_my_delay_ms(tx_delay);

	return receive;	
#endif
}

/* 
 * Handles larger delays the _delay_ms can't do by itself (not sure how accurate)  
 * Note no DBC as this function is used in the DBC !!! 
 *
 * borrowed from : https://www.avrfreaks.net/forum/delayms-problem 
 * */
void spi_my_delay_ms(unsigned int delay) 
{
	unsigned int i;

	for (i=0; i<(delay/10); i++) 
	{
		_delay_ms(10);
	}
	for (i = 0; i < delay % 10; i++)
	{
		_delay_ms(1);
	}
}


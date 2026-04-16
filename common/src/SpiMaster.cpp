#include <SpiMaster.h>
#include <avr/io.h>

void SpiMaster::init()
{
    // TODO: 1. configure SPI
    /* ---!VERY IMPORTANT!--- */
    // write 1 on CS after set CS as OUTPUT

    // in the end configure SPI
    // set MOSI, SCK - output
    // set MISO - input
    // enable SPI as master
    // check ADXL datasheet for clock frequency
    // check ADXL datasheet for clock polarity
    // check ADXL datasheet for clock phase

    DDRB |= (1 << DDB2);
    PORTB |= (1 << PORTB2);
    DDRB |= (1 << DDB3) | (1 << DDB5);
    DDRB &= ~(1 << DDB4);
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA);
}

char SpiMaster::transmit(const char &d)
{
    // TODO: 2. transmit 1 byte on SPI
    // start transmission
    // wait for transmission complete
    // return response from slave

    SPDR = d;
    while (!(SPSR & (1 << SPIF)))
    {
    }

    return SPDR;
}
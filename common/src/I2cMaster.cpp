#include <I2cMaster.h>
#include <avr/io.h>

void I2cMaster::_writeByte(const char &data)
{
    // TODO: 2. send 1 byte on I2C
    // write byte
    // send command to send byte
    // wait for ack or nack
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
}  

void I2cMaster::_readByte(char &data)
{
    // TODO: 2. read 1 byte from I2C
    // send command to read byte
    // wait for ack or nack
    // read byte

    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
    data = TWDR;
}

void I2cMaster::init()
{
    // TODO: 1. configure SPI for master mode
    // set frequency; read datasheet for SCLK frequency
    // set SCLK pin output
    // enable internal pull up resistor for SCLK and SDA, set PORTxn = 1
    TWSR = 0x00;                            // prescaler = 1
    TWBR = 72;                              // SCL frequency = F_CPU / (16 + 2 * TWBR * prescaler) = 100kHz
    TWCR = (1 << TWEN);                     // enable TWI
    PORTC |= (1 << PORTC4) | (1 << PORTC5); // enable pull-up resistors for SCL and SDA
}

void I2cMaster::sendStart()
{
    // TODO: 2. send I2C start condition
    // send start condition
    // wait for start has been transmitted
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ;
}

bool I2cMaster::writeAddrWrite(const char &addr)
{
    // TODO: 2. send slave addr for write
    // add write bit to addr {addr[6:0],r/w}
    _writeByte((addr << 1) | 0);
    // check if ack received for addr
    return (TWSR & 0xF8) == 0x18; // SLA+W transmitted and ACK received
}

bool I2cMaster::writeAddrRead(const char &addr)
{
    // TODO: 2. send slave addr for read
    // add read bit to addr {addr[6:0],r/w}
    _writeByte((addr << 1) | 1);
    // check if ack received for addr
    return (TWSR & 0xF8) == 0x40; // SLA+R transmitted and ACK received
}

bool I2cMaster::writeByte(const char &d, I2cResponse expectedResponse)
{
    // TODO: 2. send byte on I2C
    // send byte
    _writeByte(d);
    // retrun if ack received
    return (TWSR & 0xF8) == 0x28; // Data transmitted and ACK received
}

bool I2cMaster::readByte(char &d, I2cResponse expectedResponse)
{
    if (expectedResponse == ACKNOWLEDGE)
    {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Enable ACK after reception
    }
    else
    {
        TWCR = (1 << TWINT) | (1 << TWEN); // No ACK after reception (trimite NACK)
    }
    while (!(TWCR & (1 << TWINT)))
        ;     
    d = TWDR; 
    return true;
}

void I2cMaster::sendStop()
{
    // TODO: 2. send I2C stop condition
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
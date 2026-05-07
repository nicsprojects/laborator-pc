#include <I2cMaster.h>
#include <avr/io.h>

#define TW_STATUS        (TWSR & 0xF8)
#define TW_START         0x08
#define TW_SLA_ACK    0x18
#define TW_DATA_ACK   0x28
#define TW_SLA_ACK    0x40
#define TW_DATA_ACK   0x50
#define TW_DATA_NACK  0x58

void I2cMaster::_writeByte(const char& data) {
    // TODO: 2. send 1 byte on I2C
    // write byte
    TWDR = data;
    // send command to send byte
    TWCR = (1 << TWINT) | (1 << TWEN);
    // wait for ack or nack
    while (!(TWCR & (1 << TWINT)));
}

void I2cMaster::_readByte(char& data) {
    // TODO: 2. read 1 byte from I2C
    // send command to read byte
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    // wait for ack or nack
    while (!(TWCR & (1 << TWINT)));
    // read byte
    data = TWDR;
}

void I2cMaster::init() {
    // TODO: 1. configure SPI for master mode
    // set frequency; read datasheet for SCLK frequency
    // set SCLK pin output
    // enable internal pull up resistor for SCLK and SDA, set PORTxn = 1
    TWBR = 72;
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
    PORTC |= (1 << PC4) | (1 << PC5);
    TWCR = (1 << TWEN);
}

void I2cMaster::sendStart() {
    // TODO: 2. send I2C start condition
    // send start condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    // wait for start has been transmitted
    while (!(TWCR & (1 << TWINT)));
}

bool I2cMaster::writeAddrWrite(const char& addr) {
    // TODO: 2. send slave addr for write
    // add write bit to addr {addr[6:0],r/w}
    _writeByte((addr << 1) | 0x00);
    // check if ack received for addr
    return (TW_STATUS == 0x18);
}

bool I2cMaster::writeAddrRead(const char& addr) {
    // TODO: 2. send slave addr for read
    // add read bit to addr {addr[6:0],r/w}
    _writeByte((addr << 1) | 0x01);
    // check if ack received for addr
    return (TW_STATUS == 0x40);
}

bool I2cMaster::writeByte(const char& d, I2cResponse expectedResponse) {
    // TODO: 2. send byte on I2C
    // send byte
    _writeByte(d);
    // retrun if ack received
    return (expectedResponse == I2cResponse::ACK)
        ? (TW_STATUS == 0x28)
        : (TW_STATUS != 0x28);
}

bool I2cMaster::readByte(char& d, I2cResponse expectedResponse) {
    // TODO: 2. read byte from I2C
    if (expectedResponse == I2cResponse::ACK)
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    else
        TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    d = TWDR;
    // retrun if ack received
    return (expectedResponse == I2cResponse::ACK)
        ? (TW_STATUS == 0x50)
        : (TW_STATUS == 0x58);
}

void I2cMaster::sendStop() {
    // TODO: 2. send I2C stop condition
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}

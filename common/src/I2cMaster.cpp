#include <I2cMaster.h>
#include <avr/io.h>

void I2cMaster::_writeByte(const char &data)
{
    // TODO: 2. send 1 byte on I2C
    // write byte
    // send command to send byte
    // wait for ack or nack
    TWDR = data; // Load the data byte into the TWI Data Register (TWDR)
    // Clear TWINT to start the transmission and ensure TWI is enabled (TWEN)
    TWCR = (1 << TWINT) | (1 << TWEN);
    // Wait until TWINT is set by hardware indicating the transfer finished
    while (!(TWCR & (1 << TWINT)))
        ;
}

void I2cMaster::_readByte(char &data)
{
    // TODO: 2. read 1 byte from I2C
    // send command to read byte
    // wait for ack or nack
    // read byte

    // Start reception by clearing TWINT and keeping TWI enabled
    TWCR = (1 << TWINT) | (1 << TWEN);
    // Wait for the reception to complete (TWINT set)
    while (!(TWCR & (1 << TWINT)))
        ;
    data = TWDR; // Read the received byte from TWDR
}

void I2cMaster::init()
{
    // TODO: 1. configure SPI for master mode
    // set frequency; read datasheet for SCLK frequency
    // set SCLK pin output
    // enable internal pull up resistor for SCLK and SDA, set PORTxn = 1
    TWSR = 0x00;                            // Set TWI Status Register prescaler bits to 0 → prescaler = 1
    TWBR = 72;                              // Set TWI Bit Rate Register to get ~100kHz SCL with 16MHz F_CPU
    TWCR = (1 << TWEN);                     // Enable TWI by setting TWEN in TWCR
    PORTC |= (1 << PORTC4) | (1 << PORTC5); // Enable internal pull-up resistors on PC4 (SDA) and PC5 (SCL)
}

void I2cMaster::sendStart()
{
    // TODO: 2. send I2C start condition
    // send start condition
    // wait for start has been transmitted
    // Transmit START by setting TWSTA and clear TWINT to begin
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    // Wait for TWINT to be set indicating START has been transmitted
    while (!(TWCR & (1 << TWINT)))
        ;
}

bool I2cMaster::writeAddrWrite(const char &addr)
{
    // TODO: 2. send slave addr for write
    // add write bit to addr {addr[6:0],r/w}
    // Shift 7-bit address left and add write bit (0), then transmit
    _writeByte((addr << 1) | 0);
    // Check TWSR status bits masked with 0xF8 for SLA+W transmitted and ACK received (0x18)
    return (TWSR & 0xF8) == 0x18; // SLA+W transmitted and ACK received
}

bool I2cMaster::writeAddrRead(const char &addr)
{
    // TODO: 2. send slave addr for read
    // add read bit to addr {addr[6:0],r/w}
    // Shift 7-bit address left and add read bit (1), then transmit
    _writeByte((addr << 1) | 1);
    // Check TWSR status for SLA+R transmitted and ACK received (0x40)
    return (TWSR & 0xF8) == 0x40; // SLA+R transmitted and ACK received
}

bool I2cMaster::writeByte(const char &d, I2cResponse expectedResponse)
{
    // TODO: 2. send byte on I2C
    // send byte
    // Transmit the data byte
    _writeByte(d);
    // Return true if status indicates data transmitted and ACK received (0x28)
    return (TWSR & 0xF8) == 0x28; // Data transmitted and ACK received
}

bool I2cMaster::readByte(char &d, I2cResponse expectedResponse)
{
    if (expectedResponse == ACKNOWLEDGE)
    {
        // Prepare to receive a byte and reply with ACK after reception (TWEA = 1)
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA); // Enable ACK after reception
    }
    else
    {
        // Prepare to receive a byte and do not ACK (send NACK) after reception
        TWCR = (1 << TWINT) | (1 << TWEN); // No ACK after reception (send NACK)
    }
    // Wait for reception to complete
    while (!(TWCR & (1 << TWINT)))
        ;
    d = TWDR; // Read received byte from data register
    return true; // Indicate success
}

void I2cMaster::sendStop()
{
    // TODO: 2. send I2C stop condition
    // Transmit STOP condition: set TWSTO and clear TWINT to start STOP
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}


/*
==================================================
EXPLICAȚIE FIȘIER: I2cMaster.cpp
==================================================

Metoda: I2cMaster::init()
- Configurează perifericul TWI/I2C al ATmega328P.
- Setează prescalerul la 1 prin TWSR.
- Setează viteza magistralei prin TWBR.
- Activează TWI prin bitul TWEN din TWCR.
- Activează rezistențele pull-up interne pentru SDA și SCL pe PORTC4 și PORTC5.

Metoda: I2cMaster::sendStart()
- Trimite condiția START pe magistrala I2C.
- Setează biții TWINT, TWSTA și TWEN în TWCR.
- Așteaptă până când operația este terminată, verificând TWINT.

Metoda: I2cMaster::writeAddrWrite(const char &addr)
- Trimite adresa slave-ului împreună cu bitul de write.
- Forma trimisă este: adresa pe 7 biți urmată de bitul 0.
- Verifică statusul 0x18, adică SLA+W transmis și ACK primit.

Metoda: I2cMaster::writeAddrRead(const char &addr)
- Trimite adresa slave-ului împreună cu bitul de read.
- Forma trimisă este: adresa pe 7 biți urmată de bitul 1.
- Verifică statusul 0x40, adică SLA+R transmis și ACK primit.

Metoda: I2cMaster::writeByte(const char &d, I2cResponse expectedResponse)
- Trimite un byte pe magistrala I2C.
- Scrie byte-ul în TWDR.
- Pornește transmisia prin TWCR.
- Verifică statusul 0x28, adică data transmisă și ACK primit.

Metoda: I2cMaster::readByte(char &d, I2cResponse expectedResponse)
- Citește un byte de pe magistrala I2C.
- Dacă expectedResponse este ACKNOWLEDGE, trimite ACK după recepție.
- Dacă expectedResponse este NACK, nu mai confirmă ultimul byte.
- Salvează byte-ul primit din TWDR în variabila d.

Metoda: I2cMaster::sendStop()
- Trimite condiția STOP pe magistrala I2C.
- Setează biții TWINT, TWEN și TWSTO în TWCR.
- Eliberează magistrala I2C.
*/
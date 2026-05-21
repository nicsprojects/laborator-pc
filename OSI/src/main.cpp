// Objectives:
// 1. implement an OSI model for ADXL345 with I2C and SPI

#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h"
#include "Uart.h"
#include "I2cMaster.h"
#include "SpiMaster.h"


class IAdxlTransport {
public:
    virtual void writeRegister(char reg, char data) = 0;
    virtual char readRegister(char reg) = 0;
};

// TODO: 1. based on IAdxlTransport interface implement two other classes
class I2cTransport : public IAdxlTransport {
    I2cMaster i2c;
    const char addr = 0x53; // ADXL345 I2C address
public:
    I2cTransport() {
        i2c.init();
    }
    void writeRegister(char reg, char data) override {
        i2c.sendStart();
        i2c.writeAddrWrite(addr);
        i2c.writeByte(reg, ACKNOWLEDGE);
        i2c.writeByte(data, ACKNOWLEDGE);
        i2c.sendStop();
    }
    char readRegister(char reg) override {
        char data;
        i2c.sendStart();
        i2c.writeAddrWrite(addr);
        i2c.writeByte(reg, ACKNOWLEDGE );
        i2c.sendStart();
        i2c.writeAddrRead(addr);
        i2c.readByte(data, NOT_ACKNOWLEDGE);
        i2c.sendStop();
        return data;
    }
}; 

// - one SpiTransport for SPI protocol
class SpiTransport : public IAdxlTransport {
    SpiMaster spi;
public:
    SpiTransport() {
        spi.init();
    }

    void writeRegister(char reg, char data) override {
        PORTB &= ~(1 << PORTB2); // CS low
        spi.transmit(reg);
        spi.transmit(data);
        PORTB |= (1 << PORTB2); // CS high
    }
    char readRegister(char reg) override {
        PORTB &= ~(1 << PORTB2); // CS low
        spi.transmit(reg | 0x80); // set read bit
        char data = spi.transmit(0xFF);
        PORTB |= (1 << PORTB2); // CS high
        return data;
    }
};

// TODO: 2. implement Adxl high level class
class Adxl {
    IAdxlTransport* transport; 
    int16_t x, y, z;
    char lastReadAxis;
public:
    Adxl(IAdxlTransport * t) : transport(t), x(0), y(0), z(0), lastReadAxis(0) {}
    
    bool isConnected() {
        return transport->readRegister(0x00) == 0xE5; // device id register
    }
    void startMeasurement() {
        transport->writeRegister(0x2D, 0x08); 
    }
    void stopMeasurement() {
        transport->writeRegister(0x2D, 0x00); 
    }
    void readData(){
        char x0 = transport->readRegister(0x32); // data register for x-axis
        char x1 = transport->readRegister(0x33); // data register for x-axis
        char y0 = transport->readRegister(0x34); // data register for y-axis
        char y1 = transport->readRegister(0x35); // data register for y-axis
        char z0 = transport->readRegister(0x36); // data register for z-axis
        char z1 = transport->readRegister(0x37); // data register for z-axis
        
        x = (int16_t)((x1 << 8) | (x0 & 0XFF));
        y = (int16_t)((y1 << 8) | (y0 & 0XFF));
        z = (int16_t)((z1 << 8) | (z0 & 0XFF));
    }
    
    int16_t getX() {
        if (lastReadAxis != 'X') {
            readData();
            lastReadAxis = 'X';
        }
        return x;
    }
    int16_t getY() {
        if (lastReadAxis != 'Y') {
            readData();
            lastReadAxis = 'Y';
        }
        return y;
    }
    int16_t getZ() {
        if (lastReadAxis != 'Z') {
            readData();
            lastReadAxis = 'Z';
        }
        return z;
    }
};

Uart uart;
Adxl* adxl;

// TODO: 3. TEST IT!
void setup() {
    // initialize GDB stub
    uart.init();
    
    adxl = new Adxl(new SpiTransport());
    
    if (adxl->isConnected()) {
        uart.writeString("s-o conectat bAAAAAAAAAAA");
        adxl->startMeasurement();
    }
    else {
        uart.writeString("esti vai de mortii tai, nu s-a conectat");
    }
}

void loop() {
    if (adxl->isConnected()) {
       adxl->readData();
       uart.writeString("X: ");
       uart.writeIntegerNumber(adxl->getX(), 10);
       uart.writeString("Y: ");
       uart.writeIntegerNumber(adxl->getY(), 10);
       uart.writeString("Z: ");
       uart.writeIntegerNumber(adxl->getZ(), 10);
       uart.writeString("\r\n");
       delay(500);
    }
}
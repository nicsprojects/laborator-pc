// Objectives:
// 1. configure I2C
// 2. comupaction over I2C
// 3. ADXL345 usage

// Components
// ADXL345 module
// Bidirectional level shifter

// Docs
// ADXL345 Datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/adxl345.pdf

#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include <I2cMaster.h>
#include <Uart.h>

#define ADX345_I2C_ADDR 0x53

I2cMaster wire;
Uart serial;
void setup() {
    // initialize GDB stub
    serial.init();
    wire.init();

    delay(500);
    // TODO: 2. read ADXL DEVICE ID
    // expected response 0xE5
    char devid;
    wire.sendStart();
    wire.writeAddrWrite(ADX345_I2C_ADDR);
    wire.writeByte(0x00, (I2cResponse)0); // register address for DEVICE ID
    wire.sendStart();
    wire.writeAddrRead(ADX345_I2C_ADDR);
    wire.readByte(devid, (I2cResponse)0); // read DEVICE ID
    wire.sendStop();

    serial.writeString("DEVID : 0x");
    serial.writeIntegerNumber(devid, 16);
    serial.writeString("\r\n");

    delay(500);
    
    // TODO: 3. enable ADXL read
    wire.sendStart();
    wire.writeAddrWrite(ADX345_I2C_ADDR);
    wire.writeByte(0x2D, (I2cResponse)0); // register address for POWER_CTL
    wire.writeByte(0x08, (I2cResponse)0); // set MEASURE bit
    wire.sendStop();

    delay(500);
}

void ADXL_readData() {
    // TODO: 3. read X, Y, Z from ADXL and send to PC
    char buffer[6];
    wire.sendStart();
    wire.writeAddrWrite(ADX345_I2C_ADDR);
    wire.writeByte(0x32, (I2cResponse)0); // register address for DATAX0
    wire.sendStart();
    wire.writeAddrRead(ADX345_I2C_ADDR);
    for (int i = 0; i < 5; i++) {
        wire.readByte(buffer[i], (I2cResponse)1); // read byte with ACK = 1 all but last byte
    }
    wire.readByte(buffer[5], (I2cResponse)0); // read last byte with NACK = 0
    wire.sendStop();
    int16_t x = (buffer[1] << 8) | buffer[0];
    int16_t y = (buffer[3] << 8) | buffer[2];
    int16_t z = (buffer[5] << 8) | buffer[4];
    serial.writeString("X: ");
    serial.writeIntegerNumber(x, 10);
    serial.writeString("\tY: ");
    serial.writeIntegerNumber(y, 10);
    serial.writeString("Z: ");
    serial.writeIntegerNumber(z, 10);
    serial.writeString("\r\n");
}

void loop() {
    ADXL_readData();
    delay(500);
}
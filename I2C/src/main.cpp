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

#define ADX345_I2C_ADDR 0x1D  // CS -> 3.3V (LV)

#define ADXL345_REG_DEVID       0x00
#define ADXL345_REG_POWER_CTL   0x2D
#define ADXL345_REG_DATA_FORMAT 0x31
#define ADXL345_REG_DATAX0      0x32

I2cMaster wire;
Uart uart;

void ADXL_writeReg(char reg, char value) {
    wire.sendStart();
    wire.writeAddrWrite(ADX345_I2C_ADDR);
    wire.writeByte(reg,   I2cResponse::ACK);
    wire.writeByte(value, I2cResponse::ACK);
    wire.sendStop();
}

void ADXL_readRegs(char reg, char* buf, uint8_t count) {
    wire.sendStart();
    wire.writeAddrWrite(ADX345_I2C_ADDR);
    wire.writeByte(reg, I2cResponse::ACK);
    wire.sendStart();
    wire.writeAddrRead(ADX345_I2C_ADDR);
    for (uint8_t i = 0; i < count; i++) {
        I2cResponse resp = (i < count - 1) ? I2cResponse::ACK : I2cResponse::NACK;
        wire.readByte(buf[i], resp);
    }
    wire.sendStop();
}

void setup() {
    // initialize GDB stub
    debug_init();

    uart.init();
    wire.init();

    delay(500);
    // TODO: 2. read ADXL DEVICE ID
    // expected response 0xE5
    char devId = 0;
    ADXL_readRegs(ADXL345_REG_DEVID, &devId, 1);
    uart.writeString("DEVID: 0x");
    uart.writeIntegerNumber((uint8_t)devId, 16);
    uart.writeString(devId == (char)0xE5 ? " OK\r\n" : " FAIL\r\n");

    delay(500);

    // TODO: 3. enable ADXL read
    ADXL_writeReg(ADXL345_REG_DATA_FORMAT, 0x08);
    ADXL_writeReg(ADXL345_REG_POWER_CTL,   0x08);

    delay(500);
}

void ADXL_readData() {
    // TODO: 3. read X, Y, Z from ADXL and send to PC
    char raw[6];
    ADXL_readRegs(ADXL345_REG_DATAX0, raw, 6);

    int16_t x = (int16_t)(((uint8_t)raw[1] << 8) | (uint8_t)raw[0]);
    int16_t y = (int16_t)(((uint8_t)raw[3] << 8) | (uint8_t)raw[2]);
    int16_t z = (int16_t)(((uint8_t)raw[5] << 8) | (uint8_t)raw[4]);

    uart.writeString("X: "); uart.writeIntegerNumber(x, 10);
    uart.writeString(" Y: "); uart.writeIntegerNumber(y, 10);
    uart.writeString(" Z: "); uart.writeIntegerNumber(z, 10);
    uart.writeString("\r\n");
}

void loop() {
    ADXL_readData();
    delay(500);
}

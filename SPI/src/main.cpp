
// Objectives:
// 1. configure SPI
// 2. comupaction over SPI
// 3. ADXL345 usage

// Components
// ADXL345 module
// Bidirectional level shifter

// Docs
// ADXL345 Datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/adxl345.pdf

// HINTS
// use UART lib from lab2 to send messages to PC
// use itoa to convert int to string

#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints
#include <SpiMaster.h>
#include <Uart.h>

#define ADXL_CS 9
#define DEVID_ADDR 0x00
#define POWER_CTL 0x2D
#define DATAX0 0x32

Uart uart;
SpiMaster spi;

void ADXL_enable()
{
    // TODO: 2. select ADXL

    digitalWrite(ADXL_CS, LOW);
}

void ADXL_disable()
{
    // TODO: 2. deselect ADXL

    digitalWrite(ADXL_CS, HIGH);
}

char ADXL_cmdBuilder(char addr, bool rw, bool mb)
{
    return (addr & 0b00111111) | (mb << 6) | (rw << 7);
}

void setup()
{
    // initialize GDB stub
    uart.init();

    // TODO: 1. set an CS pin and make it as output
    pinMode(ADXL_CS, OUTPUT);
    ADXL_disable();

    spi.init();

    ADXL_enable();
    spi.transmit(ADXL_cmdBuilder(DEVID_ADDR, true, false));
    uint8_t devId = spi.transmit(0x00);
    ADXL_disable();

    uart.writeString("ADXL345 ID: 0x");
    uart.writeIntegerNumber(devId, 16);
    uart.writeString("\r\n");

    ADXL_enable();
    spi.transmit(ADXL_cmdBuilder(POWER_CTL, false, false));
    spi.transmit(0x08);
    ADXL_disable();

    // TODO: 2. read ADXL DEVICE ID
    // analyze SPI protocol on oscilloscope
    // expected response 0xE5

    delay(100);

    // TODO: 3.
}

void ADXL_readData()
{
    // TODO: 3. read X, Y, Z from ADXL and send to PC
    int16_t x, y, z;
    uint8_t buffer[6];

    ADXL_enable();
    spi.transmit(ADXL_cmdBuilder(DATAX0, true, true));
    for (int i = 0; i < 6; i++)
    {
        buffer[i] = spi.transmit(0x00);
    }
    ADXL_disable();

    x = (buffer[1] << 8) | buffer[0];
    y = (buffer[3] << 8) | buffer[2];
    z = (buffer[5] << 8) | buffer[4];

    uart.writeString("X: ");
    uart.writeIntegerNumber(x, 10);
    uart.writeString(" Y: ");
    uart.writeIntegerNumber(y, 10);
    uart.writeString(" Z: ");
    uart.writeIntegerNumber(z, 10);
    uart.writeString("\r\n");
}

void loop()
{
    ADXL_readData();
    delay(500);
}

/*
==================================================
EXPLICAȚIE FIȘIER: main.cpp - Laborator SPI
==================================================

Metoda: ADXL_enable()
- Selectează senzorul ADXL345 pentru comunicația SPI.
- Pune pinul CS pe LOW.
- Cât timp CS este LOW, senzorul acceptă comenzile trimise de master.

Metoda: ADXL_disable()
- Deselectează senzorul ADXL345.
- Pune pinul CS pe HIGH.
- Cât timp CS este HIGH, senzorul ignoră comunicația SPI.

Metoda: ADXL_cmdBuilder(char addr, bool rw, bool mb)
- Construiește byte-ul de comandă pentru ADXL345.
- Păstrează adresa registrului pe 6 biți.
- Adaugă bitul RW pentru citire sau scriere.
- Adaugă bitul MB pentru acces multi-byte.
- Este folosită pentru citirea sau scrierea registrelor ADXL345.

Metoda: setup()
- Inițializează UART pentru afișarea datelor.
- Inițializează SPI în mod Master.
- Configurează pinul CS.
- Citește registrul DEVID pentru verificarea senzorului.
- Activează senzorul scriind 0x08 în registrul POWER_CTL.

Metoda: ADXL_readData()
- Selectează senzorul cu CS LOW.
- Trimite comanda de citire multi-byte de la registrul DATAX0, adresa 0x32.
- Trimite bytes dummy pentru a primi datele de la senzor.
- Citește 6 bytes pentru axele X, Y și Z.
- Combină câte doi bytes pentru fiecare axă.
- Trimite valorile prin UART.

Metoda: loop()
- Citește periodic datele de accelerație.
- Afișează valorile X, Y și Z în terminal.
*/
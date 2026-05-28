#include <Uart.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>

#define F_CPU 16000000UL


void Uart::init()
{
    // TODO: 1. configure UART
    // set UART pin directions
    // set baud rate 9600
    // enable RX and TX
    // set frame format: 8 bits, 1 stop bit, no parity
    // DO NOT CHANGE ALL BITS
    DDRD &= ~(1 << DDD0);
    DDRD |= (1 << DDD1);
    int baudrate = 9600;
    uint16_t ubrr = F_CPU / 16 / baudrate - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    UCSR0C &= ~((1 << USBS0) | (1 << UPM01) | (1 << UPM00));
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void Uart::writeByte(const char &d)
{
    // TODO: 2. implement write + test it and capture on oscilloscope
    // wait to empty transmit buffer
    // write data
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = d;
}

bool Uart::available()
{
    // TODO: 3. check if are data in UART buffer
    return (UCSR0A & (1 << RXC0)) != 0;
}

char Uart::readByte()
{
    // TODO: 3. implement read
    // wait for data
    // read data and return it
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void Uart::writeString(const char *msg)
{
    for (int i = 0; i < strlen(msg); i++)
        writeByte(msg[i]);
}

void Uart::writeIntegerNumber(const int &number, const int &base)
{
    char buffer[17];
    itoa(number, buffer, base);
    writeString(buffer);
}

/*
==================================================
EXPLICAȚIE FIȘIER: Uart.cpp
==================================================

Metoda: Uart::init()
- Configurează perifericul UART0 al ATmega328P.
- Setează PD0 ca RX și PD1 ca TX.
- Calculează valoarea UBRR pentru baud rate 9600.
- Activează transmisia și recepția prin UCSR0B.
- Configurează cadrul UART: 8 biți de date, 1 bit de stop, fără paritate.

Metoda: Uart::writeByte(const char &d)
- Așteaptă până când registrul de transmisie este liber.
- Verifică flag-ul UDRE0 din UCSR0A.
- Scrie byte-ul în UDR0 pentru a fi transmis.

Metoda: Uart::available()
- Verifică dacă există date primite.
- Testează flag-ul RXC0 din UCSR0A.
- Returnează true dacă există un byte disponibil.

Metoda: Uart::readByte()
- Așteaptă până când se primește un byte.
- Citește și returnează valoarea din UDR0.

Metoda: Uart::writeString(const char *msg)
- Trimite un șir de caractere prin UART.
- Apelează writeByte() pentru fiecare caracter.

Metoda: Uart::writeIntegerNumber(const int &number, const int &base)
- Convertește un număr întreg în text folosind baza primită.
- Trimite rezultatul prin UART folosind writeString().
*/
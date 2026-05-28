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
    DDRD &= ~(1 << DDD0); // configure PD0 as input for UART RX
    DDRD |= (1 << DDD1);
    // RX pin PD0 is input for UART receive
    // configure PD1 as output for UART TX
    // TX pin PD1 is output for UART transmit
    int baudrate = 9600;
    // desired UART baud rate
    // calculate UBRR for 9600 baud with 16 MHz clock
    uint16_t ubrr = F_CPU / 16 / baudrate - 1;
    // set high byte of baud rate register
    // compute UBRR value for 9600 baud at 16 MHz
    // set low byte of baud rate register
    UBRR0H = (unsigned char)(ubrr >> 8);
    // enable UART receiver and transmitter
    // write high byte of baud rate divisor
    // configure 1 stop bit and no parity
    UBRR0L = (unsigned char)ubrr;
    // configure 8 data bits in UART frame
    // write low byte of baud rate divisor
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // enable UART receiver and transmitter
    UCSR0C &= ~((1 << USBS0) | (1 << UPM01) | (1 << UPM00));
    // configure 1 stop bit and no parity
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
    // configure character size to 8 bits
}

void Uart::writeByte(const char &d)
{
    // TODO: 2. implement write + test it and capture on oscilloscope
    // wait to empty transmit buffer
    // write data
    while (!(UCSR0A & (1 << UDRE0))); // wait until transmit buffer is empty
    UDR0 = d;
    // write the byte into UART data register for transmission
}

bool Uart::available()
{
    // TODO: 3. check if are data in UART buffer
    return (UCSR0A & (1 << RXC0)) != 0; // return true if a byte has been received
}

char Uart::readByte()
{
    // TODO: 3. implement read
    // wait for data
    // read data and return it
    while (!(UCSR0A & (1 << RXC0))); // wait until a byte is received
    return UDR0;
    // return the received byte from UART data register
}

void Uart::writeString(const char *msg)
{
    for (int i = 0; i < strlen(msg); i++) // iterate until null terminator
        writeByte(msg[i]);
}

void Uart::writeIntegerNumber(const int &number, const int &base)
{
    char buffer[17];
    // temporary buffer to hold converted integer string
    itoa(number, buffer, base);
    // convert integer to string using the specified base
    writeString(buffer);
    // send converted string over UART
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
#ifndef UART_H
#define UART_H

#define FOSC 16e6 // 16MHz
// CPU clock frequency used to compute UART baud rate

/// @brief Implement high level functions for UART protocol in ATmega328p
class Uart {
public: 
    /// @brief This function enable and configure UART peripheral. Also configure MCU pins
    void init();
    // initialize UART hardware and pin directions

    /// @brief Write one byte on TX chanel, lock until send ends
    /// @param d The 8 bits will be send over UART
    void writeByte(const char& d);
    // send one byte over UART TX

    /// @brief Check if UART RX buffer has data
    /// @return If there are data in RX buffer
    bool available();
    // return true when UART has received a byte

    /// @brief Take one data from RX buffer, lock until data available
    /// @return One byte (8 bits) from UART RX buffer
    char readByte();
    // read one byte from UART RX data register

    /// @brief Send a string over UART TX chanel
    /// @param msg The message to be send
    void writeString(const char* msg);
    // transmit a null-terminated string over UART

    /// @brief Receive an integer number, convert to string and send over UART TX
    /// @param number The integer number to be send
    /// @param base The radix in which the number will be represented
    void writeIntegerNumber(const int& number, const int& base = 10);
    // convert integer to text and send over UART
};

#endif // UART_H

/*
==================================================
EXPLICAȚIE FIȘIER: Uart.h
==================================================

Scopul fișierului:
- Declară clasa Uart.
- Clasa Uart oferă funcții de nivel mai înalt pentru comunicarea UART.
- Implementarea efectivă a metodelor se află în Uart.cpp.
- Fișierul header este folosit ca interfață între main.cpp și codul UART.

Constanta: FOSC
- Reprezintă frecvența procesorului.
- Valoarea este 16 MHz.
- Este folosită pentru calcularea baud rate-ului UART.

Clasa: Uart
- Abstractizează comunicația UART0 a microcontrolerului ATmega328P.
- Permite trimiterea și primirea de date fără ca main.cpp să lucreze direct cu registrele UART.

Metoda: void init()
- Inițializează perifericul UART.
- Configurează baud rate-ul.
- Activează transmisia și recepția.
- Configurează formatul cadrului UART.

Metoda: void writeByte(const char& d)
- Trimite un singur byte prin UART.
- Este folosită ca funcție de bază pentru transmiterea caracterelor.

Metoda: bool available()
- Verifică dacă există date primite în bufferul UART.
- Returnează true dacă se poate citi un byte.
- Returnează false dacă nu există date disponibile.

Metoda: char readByte()
- Citește un byte primit prin UART.
- Așteaptă până când un byte este disponibil.
- Returnează caracterul citit.

Metoda: void writeString(const char* msg)
- Trimite un șir de caractere prin UART.
- Apelează writeByte() pentru fiecare caracter din șir.

Metoda: void writeIntegerNumber(const int& number, const int& base = 10)
- Trimite un număr întreg prin UART.
- Numărul este convertit în text.
- Parametrul base stabilește baza de afișare, de exemplu 10 pentru zecimal sau 16 pentru hexazecimal.

Observație:
- Acest fișier doar declară funcțiile.
- Codul concret, cu registrele UCSR0A, UCSR0B, UCSR0C, UBRR0 și UDR0, este în Uart.cpp.
- Directiva #ifndef / #define / #endif previne includerea multiplă a headerului.
*/
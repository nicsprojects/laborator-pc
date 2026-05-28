// Objectives:
// 1. configure UART on ATmega328p
// 2. write bytes on UART
// 3. receive bytes on UART
// 4. text protocols

// Components
// 2 LEDs + resistors
// 1 Push Button + resistor

#include "Arduino.h"
#include <Uart.h>
#include "avr8-stub.h"
#include "app_api.h"
#include "laboratory_types.h"

#define BTN_PIN 6
#define LED0_PIN 4
#define LED1_PIN 2
#define OC1A 9


int cnr = 1;
Uart uart;
char buf[1024];
int bufindx = 0;
bool ledclipeste = 1;

void inline setLed0State(LedState state)
{
    if (state == ON)
    {
        PORTD |= (1 << LED0_PIN);
    }
    else
    {
        PORTD &= ~(1 << LED0_PIN);
    }
}

void inline setLed1State(LedState state)
{
    if (state == ON)
    {
        PORTD |= (1 << LED1_PIN);
    }
    else
    {
        PORTD &= ~(1 << LED1_PIN);
    }
}

ButtonState inline readBtnState()
{
    if (PIND & (1 << BTN_PIN))
    {
        return PRESSED;
    }
    else
    {
        return NOT_PRESSED;
    }
}

static void ledoscilat(){
    TCCR1A =(0b10<<0)|(0b11<<6);//LSB WGN modcmp
    TCCR1B =(0b11<<3)|(0b101<<0);//MSB WGM , clk
    ICR1 = 10937;
    OCR1A = 7812;
}

static void toggleLed(ButtonState btnState)
{
    static ButtonState lastState = NOT_PRESSED;
    static bool ledState = false;
    if (lastState == NOT_PRESSED &&
        btnState == PRESSED)
    {
        ledState = !ledState;
        setLed0State(ledState ? ON : OFF);
    }
    lastState = btnState;
}

void responseBack(const char *userMsg)
{
    uart.writeString("You said: ");
    uart.writeString(userMsg);
    uart.writeString("\n");
}

void processProtocol(const char *command)
{
   
    if (strcmp(command, "b") == 0)//am pus strcmp
    {
        if (readBtnState() == PRESSED)
            uart.writeString("butonu e apasat\n");
        else
            uart.writeString("butonu NU e apasat\n");
    }
    else if (strcmp(command, "i") == 0)
    {
        setLed0State(OFF);
        uart.writeString("led 0 APRINS\n");
    }
    else if (strcmp(command, "i") == 0)
    {
        setLed0State(ON);
        uart.writeString("led 0 STINS\n");
    }
    else if (strcmp(command, "c") == 0)
    {
        ledclipeste = 1;
        uart.writeString("clipeste ledu\n");
    }
    else if (strcmp(command, "s") == 0)
    {
        ledclipeste = 0;
        uart.writeString("nu mai clipeste ledu\n");
    }
    
}
void setup()
{
    uart.init();
    uart.writeByte('b');
    DDRD &= ~(1 << BTN_PIN);
    DDRD |= (1 << LED0_PIN);
    DDRD |= (1 << LED1_PIN);
    PORTD &= ~(1 << LED0_PIN);
    PORTD &= ~(1 << LED1_PIN);
    
    DDRB |=(1<<1);//PB1
    
}
void loop()
{

    toggleLed(readBtnState());
    if (ledclipeste == 1)
    {
        ledoscilat();
    }

    // TODO: 3. buffer bytes from user and when reach newline(\n) send the buffer back - use responseBack(...)
    // TODO: 4. implement a text protocol with the following minimal features:
    //  - one button to read state (pressed or not)
    //  - one led to set state (on or off)
    //  - one blinking led (500ms on, 200ms off) and enable or disable it
    // DO NOT BLOCK THE MCU
    // HINT: standalone timer, internal timer, multiple options

    if (uart.available())
    {
        char c = uart.readByte();
        if (c == '\n')
        {
            buf[bufindx] = '\0';
            responseBack(buf);
            processProtocol(buf);
            bufindx = 0; // clean buffer
        }
        else if (c != '\r')
        {
            buf[bufindx++] = c;
        }
    }

    
}


/*
==================================================
EXPLICAȚIE FIȘIER: main.cpp - Laborator UART
==================================================

Funcția: setLed0State(LedState state)
- Controlează LED0 folosind direct registrul PORTD.
- Dacă state este ON, setează bitul LED0_PIN în PORTD.
- Dacă state este OFF, șterge bitul LED0_PIN din PORTD.
- Nu folosește digitalWrite(), ci acces direct la regiștri.

Funcția: setLed1State(LedState state)
- Controlează LED1 folosind direct registrul PORTD.
- Dacă state este ON, setează bitul LED1_PIN în PORTD.
- Dacă state este OFF, șterge bitul LED1_PIN din PORTD.
- Este similară cu setLed0State(), dar lucrează pe alt pin.

Funcția: readBtnState()
- Citește starea butonului folosind registrul PIND.
- Verifică dacă bitul BTN_PIN este 1.
- Dacă bitul este 1, returnează PRESSED.
- Altfel, returnează NOT_PRESSED.
- Nu folosește digitalRead(), ci citire directă din registrul de intrare.

Funcția: ledoscilat()
- Configurează Timer1 pentru generarea unui semnal PWM pe pinul OC1A.
- Folosește registrele TCCR1A și TCCR1B pentru modul de lucru al timerului.
- Setează ICR1 ca valoare TOP.
- Setează OCR1A pentru factorul de umplere al semnalului.
- Scopul este să controleze clipirea/oscilația LED-ului folosind timer hardware.

Funcția: toggleLed(ButtonState btnState)
- Schimbă starea LED0 atunci când butonul este apăsat.
- Reține starea precedentă a butonului în variabila statică lastState.
- Detectează tranziția de la NOT_PRESSED la PRESSED.
- La fiecare apăsare validă, inversează starea LED-ului.
- Folosește variabilă statică pentru a păstra starea între apeluri.

Funcția: responseBack(const char *userMsg)
- Trimite înapoi prin UART mesajul primit de la utilizator.
- Scrie textul "You said: ".
- Apoi scrie mesajul primit.
- La final trimite caracterul newline.
- Folosește metoda uart.writeString().

Funcția: processProtocol(const char *command)
- Interpretează comenzile text primite prin UART.
- Dacă primește comanda "b", citește starea butonului și trimite răspuns prin UART.
- Dacă primește comanda pentru LED, modifică starea LED-ului.
- Dacă primește comanda "c", activează clipirea LED-ului.
- Dacă primește comanda "s", oprește clipirea LED-ului.
- Această funcție reprezintă protocolul text al laboratorului.

Funcția: setup()
- Inițializează comunicația UART prin uart.init().
- Configurează pinul butonului ca input folosind DDRD.
- Configurează pinii LED-urilor ca output folosind DDRD.
- Stinge LED-urile inițial prin PORTD.
- Configurează pinul PB1 ca output pentru funcția OC1A / Timer1.
- Pregătește microcontrolerul pentru comunicare și controlul perifericelor.

Funcția: loop()
- Rulează continuu logica principală a programului.
- Citește starea butonului și apelează toggleLed().
- Dacă ledclipeste este 1, apelează ledoscilat().
- Verifică dacă există date primite prin UART folosind uart.available().
- Dacă există un caracter primit, îl citește cu uart.readByte().
- Caracterele sunt salvate într-un buffer până la primirea caracterului '\n'.
- Când se primește newline, bufferul este închis cu '\0'.
- Apoi mesajul este trimis înapoi prin responseBack().
- După aceea comanda este interpretată prin processProtocol().
- Bufferul este resetat pentru următoarea comandă.

Ideea laboratorului:
- Programul configurează UART direct prin regiștrii ATmega328P.
- Comunicarea cu utilizatorul se face prin mesaje text.
- Programul primește comenzi prin UART și controlează LED-uri/buton.
- Nu sunt folosite funcții Arduino precum digitalWrite(), digitalRead() sau Serial.
- Controlul pinilor se face prin DDRD, PORTD și PIND.
*/
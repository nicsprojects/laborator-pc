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
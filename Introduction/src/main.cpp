// Objectives:
// 1. presenting ATmega32p datasheet
// 2. pin configuration for in/out
// 3. read/write from pins
// 4. introduction in electrical workbench equipments
//  4.1. presentation + utilization multimeter (measure R, Vd)
//  4.2. presentation + visualization on oscilloscope
//  4.3. presentation programmable power supply

// Components
// 2 LEDs + resistors
// 1 Push Button + resistor

#include "Arduino.h"
#include "avr8-stub.h"
#include "app_api.h" // only needed with flash breakpoints

#define LED0_PIN 13
#define LED1_PIN 12
#define BUTTON_PIN 2

void setup() {
    // initialize GDB stub
    debug_init();

    // TODO: 1. configure pin directions and turn off leds
    pinMode(LED0_PIN, OUTPUT);
    digitalWrite(LED0_PIN, LOW);
    pinMode(LED1_PIN, OUTPUT);
    digitalWrite(LED1_PIN, LOW);
    pinMode(BUTTON_PIN, INPUT);
}

void inline setLed0State(LedState state) {
    // TODO: 1. turn led0 on or off based on state
    digitalWrite(LED0_PIN, state == LedState::ON ? HIGH : LOW);
}

// read button state
ButtonState inline readBtnState() {
    // TODO: 1. return if button is pressed
    return digitalRead(BUTTON_PIN) == HIGH ? ButtonState::PRESSED : ButtonState::NOT_PRESSED;
}

void inline setLed1State(LedState state) {
    // TODO: 2. turn led1 on or off based on state
    digitalWrite(LED1_PIN, state == LedState::ON ? HIGH : LOW);
}

static void oscillatingLed() {
    // TODO: 2. toggle the led1 with the pattern: 500ms on, 200ms off
    // TODO: 3. analyze on oscliisop the  led1 signal and measure the timing
    static unsigned long lastToggle = 0;
    static bool led1State = false;
    static const int onTime = 500;
    static const int offTime = 200;
    unsigned long now = millis();
    if (led1State && now - lastToggle >= onTime) {
        led1State = false;
        setLed1State(LedState::OFF);
        lastToggle = now;
    } else if (!led1State && now - lastToggle >= offTime) {
        led1State = true;
        setLed1State(LedState::ON);
        lastToggle = now;
    }
}

// detect button press and toggle the led0 state
static void toggleLed(ButtonState btnState) {
    static bool lastState = ButtonState::NOT_PRESSED;
    static bool ledState = false;
    if ( lastState == ButtonState::NOT_PRESSED && 
            btnState == ButtonState::PRESSED ) {
        ledState = !ledState;
        setLed0State(ledState ? LedState::ON : LedState::OFF);
    }
    lastState = btnState;
}

void loop() {
    toggleLed(readBtnState());
    oscillatingLed();
    delay(100);
}
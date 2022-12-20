#include "led.h"

/*
 * state:
 * seta o estado do led
 */
void LED::state(bool state) {
    digitalWrite(pin, state);
}

/*
 * init:
 * configura o pino do led
 */
void LED::init(uint8_t pin) {
    
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

/*
 * blink:
 * pisca o led 3X e encerra o timer
 */
void LED::blink(hw_timer_t *timer) {
    static uint8_t blinkAmount = 6;
    static uint8_t i = 1;
    if (i >= blinkAmount) {
        timerEnd(timer);
        i = 0;
    }
    digitalWrite(pin, !digitalRead(pin));
    i++;
}
#include "button.h"

/*
 * init:
 * configura o pino do botao
 */
void Button::init(uint8_t pin) {
    pinMode(pin, INPUT_PULLUP);
    this->pin = pin;
}

/*
 * monitor:
 * recebe o padrao de comparacao (LOW ou HIGH)
 * caso o botao fique no estado de comparacao por ao menos 500ms
 * retorna 1
 */
bool Button::monitor(bool compare) {
    int reading = digitalRead(pin);
    if(reading != lastState) {
        timeout = millis();
    }

    if ((millis() - timeout) > 500) {
        if (reading != currentState) {
            currentState = reading;

            if (currentState == compare) {
                return 1;
            }
        }
    }

    lastState = reading;
    return 0;
}
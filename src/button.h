#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>
#include "pinout.h"

class Button {
    private:
        uint8_t lastState = HIGH;
        uint8_t currentState;
        uint8_t timeout = 0;
        uint8_t pin;
    public:
        bool monitor(bool);
        void init(uint8_t);
};

#endif
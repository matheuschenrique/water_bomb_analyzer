#ifndef _LED_H_
#define _LED_H_

#include <Arduino.h>

class LED
{
    private:
        uint8_t pin;
    public:
        uint8_t mode = 0;
        void init(uint8_t pin);
        void state(bool state);
        void blink(hw_timer_t *);
};

#endif
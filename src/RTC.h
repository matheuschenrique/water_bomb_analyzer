#ifndef _RTC_H_
#define _RTC_H_

#include <Arduino.h>
#include <Wire.h>
#include "RTClib.h"
#include <string.h>

class RTC
{
private:
    RTC_DS1307 rtc;
    DateTime now;
    int day, month, year, hour, minutes, seconds;
public:
    void begin();
    String get_date(char = '/');
    String get_time(char = ':');
    void reconfig();
};

#endif
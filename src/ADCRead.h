#ifndef _ADCREAD_H_
#define _ADCREAD_H_

#include <Arduino.h>
#include "EmonLib.h"
#include <driver/adc.h>
#include "pinout.h"
#include "SDcard.h"
#include "RTC.h"

// Force EmonLib to use 10bit ADC resolution
#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)

#define SAMPLES 6

class ADCRead
{
    private:
        EnergyMonitor SCT013;
        bool state;
        bool change_state_flag = 0;
        bool previous_calibration_state = 0;
        double current_samples[SAMPLES];
        double last_trigger;
        bool current_fill();
        double sample_mean_calc(double *, size_t);
        void array_sample_shift(double *, size_t);
        void initial_state();
        void current_check();
        void calibration(void (*set_timer)());
    public:
        RTC rtc;
        SDcard SD;
        int calibration_state = 0;
        double smallest;
        double threshold;
        uint8_t mode = 0;
        uint8_t monitor(void (*set_timer)());
        bool get_state() { return state; }
        void set_calibration();
        void set_threshold(double);
        void write_state();
        bool begin(uint8_t, double, double);
};

#endif
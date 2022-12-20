/*
    initial state bugado
    botao bugado
    potenciometro não funciona
*/
#include <Arduino.h>
#include "pinout.h"
#include "ADCRead.h"
#include "led.h"
#include "button.h"

ADCRead ADC;
LED led;
Button button;
hw_timer_t *timer = NULL;

void set_timer();
void button_callback();
void cb_timer();

void setup() {
    Serial.begin(9600);

    // Wait for USB Serial
    while (!Serial) {
        yield();
    }

    led.init(LEDpin);
    led.state(HIGH);

    button.init(BUTTONpin);

    if (!ADC.begin(pinSCT, 270, 0.85)) {
        // fica preso no loop caso a inicializacao do sd card falhe
        while(1);
    }
    ADC.set_threshold(ADC.SD.read_setup());     // define o threshold lendo o valor em setup.txt
    led.state(LOW);
}

void loop() {
    if (ADC.monitor(set_timer) != 0xFF) {
        Serial.printf("%s\t%s\t%d\n", ADC.rtc.get_date(), ADC.rtc.get_time(), ADC.get_state());
        led.state(ADC.get_state());
    }

    if(button.monitor(LOW)) {
        ADC.calibration_state = !ADC.calibration_state;
    }
}

void set_timer() {
    //inicialização do timer. Parametros:
    /* 0 - seleção do timer a ser usado, de 0 a 3.
      80 - prescaler. O clock principal do ESP32 é 80MHz. Dividimos por 80 para ter 1us por tick.
    true - true para contador progressivo, false para regressivo
    */
    timer = timerBegin(0, 80, true);

    /*conecta à interrupção do timer
     - timer é a instância do hw_timer
     - endereço da função a ser chamada pelo timer
     - edge=true gera uma interrupção
    */
    timerAttachInterrupt(timer, &cb_timer, true);

    /* - o timer instanciado no inicio
       - o valor em us para 1s
       - auto-reload. true para repetir o alarme
    */
    timerAlarmWrite(timer, 100000, true); 

    //ativa o alarme
    timerAlarmEnable(timer);

}

void cb_timer(){
    led.blink(timer);
}
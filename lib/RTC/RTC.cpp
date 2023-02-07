#include "RTC.h"

/*
 * reconfig:
 * chamar a funcao quando for recalibrar o horario
 * inserir data e hora base nos campos correspondentes
 * ex: data = "Dec 21 2022"
 *     hora = "20:30:00"
 */
void RTC::reconfig(const char* data, const char* hora) {
    Serial.println("Reconfigurando horario");
    rtc.adjust(DateTime(data, hora));
}

/*
 * begin:
 * inicia o RTC
 * caso esteja com uma data muito atrasa chama reconfig
 * esta reconfiguracao sera generica apenas para ser mais proximo do real
 * nesse caso deve-se reconfigurar manualmente para ter o horario preciso
 */
void RTC::begin() {
    Serial.println("Iniciando RTC...");
    while (!rtc.begin()) {
        Serial.println("RTC nao encontrado!");
        delay(100);
    }
    Serial.println("RTC Pronto!");
    now = rtc.now();
}

/*
 * get_date
 * retorna a data
 * pode receber um separador diferente de /
 */
String RTC::get_date(char separator) {
    now = rtc.now();
    day = now.day();
    month = now.month();
    year = now.year();

    String data = (String)day + (String)separator + (String)month + (String)separator + (String)year;
    return data;
}

/*
 * get_time
 * retorna ao horario
 * pode receber um separador diferente de :
 */
String RTC::get_time(char separator) {
    now = rtc.now();
    hour = now.hour();
    minutes = now.minute();
    seconds = now.second();

    String hora = (String)hour + (String)separator + (String)minutes + (String)separator + (String)seconds;
    return hora;
}
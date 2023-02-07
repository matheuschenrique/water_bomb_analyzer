/*
    SCT-013 (1:2000)
    Vcc_ESP = 3.3V
    Corrente de pico no primario = Ipk1 = Irms*sqrt(2) = 100A*1.414 = 141.4A
    Corrente de pico no secundario = Ipk2 = Ipk1/2000 = 141.4/2000 = 0.0707A
    Resistor ideal = (Vref/2)/Ipk2 = (3,3/2)/0.0707 = 23.33 Ohm
    Utilizar resistor de 22 Ohm
*/

#include "ADCRead.h"

/*
 * mapdouble:
 * realiza mapeamento de um range para outro de um valor do tipo double
 */
double mapdouble(const double &x, const double &in_min, const double &in_max, const double &out_min, const double &out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*
 * current_fill:
 * preenche o vetor inteiro com leituras de corrente
 * deve ser usado na inicializacao
 */
bool ADCRead::current_fill() {
    static size_t i = 1;
    if (i > SAMPLES) {
        mode = 2;
        return 1;
    }
    current_samples[SAMPLES - i] = SCT013.calcIrms(5000);
    sum += current_samples[SAMPLES - i];
    i++;
    return 0;
}

/*
 * current_check
 * monitora o valor de corrente
 * adiciona uma nova amostra no inicio do vetor
 * calcula a nova media retirando o offset de calibracao (smallest)
 */
void ADCRead::current_check() {
    static int sample_count = 0;
    double mean = 0;
    double new_sample = SCT013.calcIrms(8000);

    sum -= current_samples[sample_count];  // remove a amostra presente na posicao atual
    current_samples[sample_count] = new_sample; // substitui pela nova amostra
    sum += new_sample; // incrementa sum com a nova amostra
    mean = sum / SAMPLES;

    if (sample_count >= SAMPLES) {
        sample_count = 0;
    } else {
        sample_count++;
    }

    Serial.print("Corrente: ");
    Serial.println(mean - smallest);

    /* ao detectar uma alteracao no valor maior ou menor que o threshold
     * seta a flag para indicar esta mudanca
     */
    if ((mean - smallest > threshold) && (state == 0)) {
        state = 1;
        Serial.print("Corrente RMS: ");
        Serial.println(mean - smallest);
        change_state_flag = 1;
        last_trigger = mean - smallest;
    } else if ((mean - smallest <= threshold) && (state == 1)) {
        state = 0;
        Serial.print("Corrente RMS: ");
        Serial.println(mean - smallest);
        change_state_flag = 1;
        last_trigger = mean - smallest;
    }
}

/*
 * begin:
 * inicializa as configuracoes para o ADC
 * inicializa cartao SD e RTC
 * configura o potenciometro de regulagem
 */
bool ADCRead::begin(uint8_t pin, double calibration, double small) {

    if (!SD.begin()) {
        return 0;
    }
    rtc.begin();

    Serial.println("Iniciando ADC");

    pinMode(POTpin, INPUT);
    
    adc2_config_channel_atten(ADC2_CHANNEL_3, ADC_ATTEN_DB_11); // POT
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // SCT013
    analogReadResolution(10);

    SCT013.current(pin, calibration);
    
    this->smallest = small;
    return 1;
}

/*
 * set_threshold:
 * atualiza o valor do threshold
 * seu valor sera setado em toda inicializacao com base no conteudo de setup.txt
 */
void ADCRead::set_threshold(const double threshold) {
    this->threshold = threshold;
    Serial.println("Threshold atual: ");
    Serial.println(threshold);
}

/*
 * initial_state:
 * calcula o estado inicial do sensor apos completar a array
 */
void ADCRead::initial_state() {
    if ((sum / SAMPLES) - smallest > threshold) {
        state = 1;
    } else {
        state = 0;
    }
    change_state_flag = 1;
}

/*
 * calibration:
 * realiza a calibracao do threshold baseado na leitura do potenciometro
 * salva o valor no arquivo setup.txt que sera lido em toda inicializacao
 * recebe a funcao set_timer como argumento
 * set_timer reseta o timer para realizar controle do led de status
 */
void ADCRead::calibration(void (*set_timer)()) {

    if (previous_calibration_state == 1 && calibration_state == 0) {
        previous_calibration_state = 0;
        SD.write_setup(rtc.get_date(), rtc.get_time(), threshold);
        set_timer();
    } else if (previous_calibration_state == 0 && calibration_state == 1) {
        set_timer();
    }
    
    if(calibration_state) {
        previous_calibration_state = 1;
        threshold = mapdouble(analogRead(POTpin), 0, 1023, 0, 4);
        Serial.print("POT: ");
        Serial.println(threshold);
    }
}

/*
 * monitor:
 * funcao chamada no loop
 * maquina de estados para incializacao e leitura do sensor
 * controla o fluxo de escrita no cartao de memoria
 * recebe a funcao set_timer que sera usada para controle do led de status
 */
uint8_t ADCRead::monitor(void (*set_timer)()) {
    switch (mode)
    {
    case 0:
        (void) SCT013.calcIrms(40000); // discarta primeira amostra
        mode = 1;
        break;
    case 1:
        if(current_fill()) {
            mode = 2;
            set_timer();    // pisca o led sinalizando que terminou a inicializacao
        }
        break;
    case 2:
        initial_state();
        mode = 3;
        break;
    case 3:
        current_check();
        break;
    default:
        break;
    }

    if(change_state_flag) { 
        change_state_flag = 0;
        write_state();
        return state;
    }

    calibration(set_timer);

    return 0xFF;
}

/*
 * write_state:
 * escreve no SDCard a informacao do novo status
 * nome do arquivo: DD-MM-AA.txt
 * formato: DD/MM/AA    HH:MM:SS    estado  corrente
 */
void ADCRead::write_state() {
    String file_name;
    file_name = rtc.get_date('-') + ".txt";
    String info;
    info = rtc.get_date() + (String)"\t" + rtc.get_time() + (String)"\t" + (String)state + (String)"\t" + (String)last_trigger;
    SD.fileWrite(file_name.c_str(), &info, 1, 0);
}
#include "SDcard.h"

/*
 * begin:
 * inicializa o SDCard
 * realiza 3 tentativas de inicializacao
 */
bool SDcard::begin() {
    for (uint8_t i = 0; i < 3; i++) {
        if (!SD.begin(SS, SPI_SPEED)) {
            if (SD.card()->errorCode()) {
                Serial.println("Falha na inicializacao do cartao");
                Serial.println(int(SD.card()->errorCode()));
                Serial.println(int(SD.card()->errorData()));
            }
        } else {
            Serial.println("Cartao inicializado com sucesso");
            return 1;
        }
    }

    return 0;
}

/*
 * fileWrite:
 * abre ou cria um arquivo com o nome recebido
 * escreve no arquivo as linhas recebidas (tam = quantidade de linhas para escrever)
 * rewrite 1 indica que o arquivo sera rescrito desde o inicio, caso contrario as linhas serao apensadas no final
 */
bool SDcard::fileWrite(const char *name, const String *lines, uint16_t tam, uint64_t rewrite) {
    myFile = SD.open(name, FILE_WRITE); // Cria / Abre arquivo .txt
    if (rewrite == 1) {
        myFile.seekSet(0);
    }

    if (myFile) { // Se o Arquivo abrir imprime:
        Serial.println("Escrevendo no Arquivo .txt"); // Imprime na tela

        for (uint16_t i = 0; i < tam; i++) {
            myFile.println(*lines);
            lines++;
        }
        myFile.close(); // Fecha o Arquivo após escrever
        Serial.println("Terminado."); // Imprime na tela
        Serial.println(" ");
        return 1;
    } else { // Se o Arquivo não abrir
        Serial.println("Erro ao Abrir Arquivo .txt"); // Imprime na tela
        return 0;
    }
}

/*
 * fileRead:
 * funcao para ler um arquivo
 * no momento nao eh utilizada
 */
char SDcard::fileRead(const char *name) {
    myFile = SD.open(name); // Abre o Arquivo
    Serial.println("Abrindo arquivo");

    if (myFile) {
        Serial.println("Conteúdo do Arquivo:"); // Imprime na tela

        while (myFile.available()) { // Exibe o conteúdo do Arquivo
            Serial.write(myFile.read());
        }

        myFile.close(); // Fecha o Arquivo após ler
    } else {
        Serial.println("Erro ao Abrir Arquivo .txt"); // Imprime na tela
    }
    Serial.println("Arquivo fechado");
    return 0;
}

/*
 * write_setup:
 * escreve o arquivo setup
 * insere um cabecalho e o valor do threshol
 * EX:
 * DATA       HORA     THRESHOLD
 * 16/12/2022 23:55:12 1.25
 */
bool SDcard::write_setup(String data, String hora, double threshold) {
    String setup[2];

    setup[0] = "DATA\tHORA\tTHRESHOLD";
    setup[1] = data + (String)"\t" + hora + (String)"\t" + (String)threshold;

    Serial.println("Iniciando setup");
    fileWrite("setup.txt", setup, 2, 1); // chama fileWrite com rewrite = 1 para rescrever arquivo do inicio

    return 1;
}

/*
 * read_setup:
 * realiza leitura de setup.txt retornando o valor do threshold como double
 */
double SDcard::read_setup() {

    // Maximum line length plus space for zero byte.
    const size_t LINE_DIM = 50;
    uint8_t i = 0;
    uint8_t j = 0;

    char line[LINE_DIM];

    String value = "";

    myFile = SD.open("setup.txt"); // Abre o Arquivo
    for (i = 0; i < 3; i++){
        if (myFile) {
            myFile.fgets(line, sizeof(line));
            myFile.fgets(line, sizeof(line));
            myFile.close(); // Fecha o Arquivo após ler

            char n;
            uint8_t cop = 50; // protecao para nao ficar preso no while
            
            // encontra threshold baseado na quantidade de tabs '/t' na segunda linha
            while((n != '\n') && (cop > 0)) {
                n = line[i];
                i++;
                
                if (n == '\t') {
                    j++;
                }
                if (j == 2) {
                    value += String(n);
                }

                cop--;
            }
            myFile.close();
            return value.toDouble();
        } else {
            Serial.println("Erro ao Abrir setup.txt"); // Imprime na tela
        }
    }
    return 0;
}
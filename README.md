# water_bomb_analyzer

O objetivo do projeto é utilizar um sensor de corrente para ler os momentos em que uma bomba liga ou desliga e salvar seu resultado em um cartão SD.

## Componentes utilizados:

    1) ESP32
    2) Módulo leito de SD Card
    3) RTC DS1307
    4) Leitor de corrente SCT-013

Para sua utilização também foi realizada a leitura de um botão e um potenciômetro.  

Ao apertar o botão por um período se inicia a leitura do potenciômetro, sua leitura é utilizada para a implementação de um limiar de corrente configurável, que é salvo no SDCard no arquivo setup.txt

## Feedback do LED

Ao iniciar corretamente o LED deve piscar uma vez, ficando um tempo apagado e, após cerca de 5 segundos deve piscar três vezes, indicando que a inicialização foi finalizada.  

Caso não sera identificado um SDCard o LED acenderá assim que o sistema for ligado, não completando o processo de inicialização.  

Ao pressionar o botão por cerca de 2 segundos o LED deverá piscar 3 vezes, o que indica que uma nova regulagem do limiar foi inicializada. A regulagem só sera finalizada após pressionar o botão novamente, sendo observado um novo feedback do LED.

## Formato dos arquivos

Dois tipos de arquivos são gravados, o primeiro diz respeito à configuração de limiar de corrente, chamado de `setup.txt`, seguindo o seguinte exemplo:

    DATA        HORA        THRESHOLD
    16/12/2022  20:54:20    1.5

Todos os termos são separados por um tab.  

O segundo tipo é o arquivo que contém os horários de acionamento e desacionamento da bomba, seu nome sempre conterá o dia correspodente, por exemplo `16-12-2022.txt`, seu conteúdo seguirá o exemplo a seguir:  

    16/12/2022  20:55:52    0   0.2
    16/12/2022  20:57:33    1   2.6

O valor da terceira coluna correspondeo ao estado da bomba, sendo 1 ligado e 0 desligado, já a última coluna é o valor lido pelo sensor.
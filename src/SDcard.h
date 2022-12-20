#ifndef _SDCARD_H_
#define _SDCARD_H_

#include <Arduino.h>
#include <SPI.h>
#include "SdFat.h"

#define SPI_SPEED SD_SCK_MHZ(4)

class SDcard
{
private:
    SdFs SD;
    FsFile myFile;
public:
    bool begin();
    bool fileWrite(const char *, const String *, uint16_t, uint64_t);
    char fileRead(const char *);
    bool write_setup(String, String, double threshold);
    double read_setup();
};

#endif
#ifndef __SDCARD_H__
#define __SDCARD_H__

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// returns list of filenames as a string
String listDir(fs::FS &fs, const char *dirname, uint8_t levels);

int scanFile(fs::FS &fs, const char *path);

void readLines(fs::FS &fs, int lineNum, int lineCount, const char *path);


#endif // __SDCARD_H__
/*
Setlist Pedal Pilot
- Reads user-selected text file from SD card and scrolls through with buttons
- Upload and update files over wifi
- Connects to NTP server to update and display time, otherwise uses RTC module

Author: Tom Kuzma
Date: April 28, 2022
 */

#include "SDCard.h"
#include "DailyStruggleButton.h"
#include "SPI.h"

#define UP_BUTTON       16
#define DOWN_BUTTON     17

#define PRESS_TIME      1000
#define DEBOUNCE_TIME   20  

void buttonEvent_up (byte btnStatus);
void buttonEvent_down (byte btnStatus);

int lineCount;
int startingLine = 0;

DailyStruggleButton scrollUp;
DailyStruggleButton scrollDown;

void setup()
{
    Serial.begin(115200);

    //////////////////////////////////////////////
    //       SD Card Stuff                      //
    //////////////////////////////////////////////
    if (!SD.begin())
    {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC){
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD){
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC){
        Serial.println("SDHC");
    }
    else{
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    listDir(SD, "/", 0);

    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

    lineCount = scanFile(SD, "/test.txt");
    readLines(SD, startingLine, lineCount, "/test.txt");

    //////////////////////////////////////////////
    //      Buttons                             //
    //////////////////////////////////////////////
    scrollUp.set(UP_BUTTON, buttonEvent_up, INT_PULL_UP);
    scrollUp.setDebounceTime(DEBOUNCE_TIME);
    scrollUp.enableLongPress(PRESS_TIME);

    scrollDown.set(DOWN_BUTTON, buttonEvent_down, INT_PULL_UP);
    scrollDown.setDebounceTime(DEBOUNCE_TIME);
    scrollDown.enableLongPress(PRESS_TIME);

} // end setup

void loop()
{
    // button polling
    scrollUp.poll();
    scrollDown.poll();
} // end loop

// 
void buttonEvent_up (byte btnStatus){
    switch (btnStatus)
    {
    case onPress:
        if (startingLine != 0) {
            readLines(SD, --startingLine, lineCount, "/test.txt"); // decrement starting line and get 3 lines
        }
        break;

    case onLongPress:
        // do nothing currently
    break;
    
    default:
        break;
    }
} // end buttonEvent_up

void buttonEvent_down (byte btnStatus){
    switch (btnStatus)
    {
    case onPress:
        if (startingLine < lineCount - 2){
            readLines(SD, ++startingLine, lineCount, "/test.txt"); // increment starting line and get 3 lines
        }
        break;

    case onLongPress:
        // lineCount = scanFile(SD, "/test.txt");
        Serial.println(listDir(SD, "/", 0));
    break;
    
    default:
        break;
    }
} // end buttonEvent_down

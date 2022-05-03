#include "SDCard.h"

#define MAX_LINES 100

static int lineLocation[MAX_LINES];

String listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    //Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return "Failed to open directory";
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return "Not a directory";
    }

    File file = root.openNextFile();
    String fileName, fileList;
    while (file)
    {
        // if (file.isDirectory())
        // {
        //     // Get filename
        //     fileName = file.name();

        //     // Ommit OSX directory hidden files
        //     if (!fileName.startsWith(".")) {
        //     Serial.print("  DIR: ");
        //     Serial.print(fileName);
        //     } 
            
        //     if (levels)
        //     {
        //         listDir(fs, file.path(), levels - 1);
        //     }
        // }
        // else
        // {
            // Get filename
            fileName = file.name();

            // Ommit OSX directory hidden files
            if (!fileName.startsWith(".")) {
                fileList += (fileName + '\n');
            }
        // }
        file = root.openNextFile();
    }

    return fileList;
} // end listDir

int scanFile(fs::FS &fs, const char *path){

    int position = 0;
    int lineCount = 0;
    memset(lineLocation, 0, MAX_LINES); // clear previous line location indexes

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return -1;
    }

    // index positions of lines
    while (file.available())
    {
        if (file.read() == '\n')
        {
            lineCount++;
            lineLocation[lineCount] = position + 1;
        }
        position++;
    }

    Serial.printf("Line Count: %d\n", lineCount);

    return lineCount;
} // end scanFile


void readLines(fs::FS &fs, int lineNum, int lineCount, const char *path)
{
    // open file
    File file = fs.open(path);

    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    String line = "";

    // go to starting position of desired line
    file.seek(lineLocation[lineNum]);

    if (lineNum >= lineCount - 3) // if starting within last 3 lines
    {
        while (file.available())
        {
            line += (char)file.read();
        }
            // Add End of File tag
            line.setCharAt(line.length() - 1, '\0');
            line += "END OF SETLIST";
    }
    else
    {
        // get 3 lines from starting position
        while (file.position() < lineLocation[lineNum + 3])
        {
            line += (char)file.read();
        }
    }

    Serial.println(line);

    //    Serial.print("Read from file: ");
    //    while(file.available()){
    //        Serial.write(file.read());
    //    }

    file.close();

    // eventually need to make this return the lines to send it to the screen method
    //return line; 
}

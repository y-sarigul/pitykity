#include "FS.h"
#include "SPIFFS.h"
#include "defines.h"

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true

char *readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);
    char buff[256];
    int i;

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return NULL;
    }

    Serial.println("- read from file:");
    i = 0;
    while(file.available()){
        buff[i] = file.read();
        Serial.write(file.read());
        i++;
    }
    buff[i] = '\0';
    file.close();
    return (buff);
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}


void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

/*
 * file setup file sistem tanimlar
*/

void file_setup(){
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    writeFile(SPIFFS, "/data.txt", "");
}

// dosya yazma
// /data.txt => path

void write_file(char *line)
{
    writeFile(SPIFFS, "/data.txt", line);
}

//dosya okuma
// /data.txt => path
void read_file()
{
    readFile(SPIFFS, "/data.txt");
}

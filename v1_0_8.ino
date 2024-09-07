/*Icarus v1.0.8
Testing logging system, include BME280, RTC1307, and SD card module
*/

//LIBRARY
#include <Wire.h>
#include <SPI.h>

//Data System
#include <DS1307.h>
#include <FS.h>
#include <SD.h>

//Sensor System
#include <Adafruit_BME280.h>


//BME280 Initialization
Adafruit_BME280 bme(4);

//DS1307 Initialization
DS1307 rtc;
uint8_t sec, minute, hour, day, month;
uint16_t year;

//Environmental Variable
#define SEALEVELPRESSURE_HPA (1013.25)

void setup() {
  //Serial Setup
  Serial.begin(115200);
  Serial.println("ICARUS v1.0.6");

  //SDcard Setup
  bool sd_status = SD.begin(5);
  if (sd_status) {
    Serial.println("SD-card Module detected");
    writeFile(SD, "/flightdata.txt", "ICARUS FLIGHT COMPUTER V1.0.8\n");
  } else {
    Serial.println("Unable to detected SD-card Module");
    while(1);
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
   // need to add a failsafe system to check if there are data in the SD before overwrite
  
  //Barometer Setup
  bool bme_status = bme.begin();
  if (bme_status) {
    Serial.println("Barometer BME280 detected");
    appendFile(SD, "/flightdata.txt", "Barometer Online");
  } else {
    Serial.println("Unable to detected Barometer BME280");
    appendFile(SD, "/flightdata.txt", "Barometer Offline\n");
    while(1);
  }

  //RTC Setup
  rtc.begin();
  rtc.start();


  Serial.println();
  delay(1000);
}

void loop() {
  rtc.get(&sec, &minute, &hour, &day, &month, &year);
  char DataString[100];
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  sprintf(DataString, "%02d:%02d:%02d,%.2f\n", hour, minute, sec, altitude);

  // Print the formatted string (optional for debugging)
  Serial.println(DataString);
  appendFile(SD, "/flightdata.txt", DataString);


  /*
  Serial.print(hour, DEC);
  Serial.print(":");
  Serial.print(minute, DEC);
  Serial.print(":");
  Serial.print(sec, DEC);
  Serial.print(",");
  Serial.println(bme.readAltitude(SEALEVELPRESSURE_HPA));

  
  */

  // BMEValues();
  delay(1000);

}

//BME280 Function(s)
void BMEValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");
  
  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");
  
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  
  Serial.println();
}

//SD Function(s) - https://randomnerdtutorials.com/esp32-microsd-card-arduino/
//List directory, use listDir()
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
//Create a directory, createDir()
void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}
//Remove a directory, removeDir()
void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}
//Read File, readFile()
void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}
// WriteFile, writeFile()
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}
// Append file, appendFile()
void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
//Rename File, renameFile()
void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}
//Detele File, deleteFile()
void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}
//Test File, testFileI0()
void testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}


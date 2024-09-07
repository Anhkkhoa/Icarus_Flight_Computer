/* ICARUS v1.0.2, Arduino, I2C
This code for test Barometer BME280, using I2C (do have SPI protocol)
*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/* define pin leg for SPI
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
*/

#define SEALEVELPRESSURE_HPA (1013.25) //update code to auto calibrate to current sea level

Adafruit_BME280 bme; // I2C

//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

void setup() {
   Serial.begin(115200);
  Serial.println("ICARUS v1.0.2 - BME280 Testing");

  bool BME_status = bme.begin();
  if (BME_status) {
    Serial.println("Barometer BME280 Detected");
  } else {
    Serial.println("Unable to Detect Barometer BME280");
    while (1); 
  }       
  
  delay(1000); // waiting for loading after start up

  Serial.println();
}

void loop() { 
  printValues();
  delay(100);
}


void printValues() {
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
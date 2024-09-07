

/* ICARUS v1.0.5

*/

//library
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//BME280 Initialization
#define BME_SCK 52
#define BME_MOSI 51
#define BME_MISO 50
#define BME_CS 9
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

//BNO055 Initialization
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//Fire Button
const int Fire1pin = 6;
const int Fire2pin = 7;
int Fire1 = 0;
int Fire2 = 0;

// temporary switch condition for apogee
const int ApogeePin = 8;
int Apogee = 0; // binary stage apogee stage

//Environmental Variable
#define SEALEVELPRESSURE_HPA (1013.25)

void setup() {
  Serial.begin(115200);
  Serial.println("ICARUS v1.0.5v");

  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);

  pinMode(Fire1pin, INPUT);
  pinMode(Fire2pin, INPUT);
  pinMode(Apogee, INPUT);

  //replace this part with condition that stop running it after finish calibrate and switch to green
  digitalWrite(4, HIGH);  
  delay(100);                      
  digitalWrite(4, LOW);   
  delay(100);                      
  digitalWrite(3, HIGH);  
  delay(100);                     
  digitalWrite(3, LOW);   
  delay(100);                    
  digitalWrite(2, HIGH);  
  delay(100);                      
  digitalWrite(2, LOW);  
  delay(100); 
  digitalWrite(4,HIGH); 

  bool bme_status = bme.begin();
  if (bme_status) {
    Serial.println("Barometer BME280 detected");
  } else {
    Serial.println("Unable to detected Barometer BME280");
    while(1);
  }

  bool bno_status = bno.begin();
  if (bno_status) {
    Serial.println("IMU BNO055 detected");
    bno.setExtCrystalUse(true);
  } else {
    Serial.println("Unable to detected IMU BNO055");
    while(1);
  }

  Serial.println();
  delay(1000);

}

void loop() {
  Fire1 = digitalRead(Fire1pin);
  Fire2 = digitalRead(Fire2pin);
  Apogee = digitalRead(ApogeePin);

  if (Fire1 == HIGH & Fire2 == HIGH) { //FIRE!!!, switch to launch button system next
    digitalWrite(4,LOW);
    digitalWrite(3, HIGH);
  }

  if (Apogee == HIGH) { //switch to condition when reach Apogee
    digitalWrite(3,LOW);
    digitalWrite(2,HIGH);
  }

  BMEValues();
  BNOValues();
  delay(1000);
}

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

void BNOValues() {
  sensors_event_t event; 
  bno.getEvent(&event);
  
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");

  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  //Acceleration m/s^2
  Serial.print("Accel X: ");
  Serial.print(accel.x());
  Serial.print("\tAccel Y: ");
  Serial.print(accel.y());
  Serial.print("\tAccel Z: ");
  Serial.print(accel.z());
  Serial.println("");

  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  //Gyroscope rad/s
  Serial.print("Gyro X: ");
  Serial.print(gyro.x());
  Serial.print("\tGyro Y: ");
  Serial.print(gyro.y());
  Serial.print("\tGyro Z: ");
  Serial.print(gyro.z());
  Serial.println("");

  Serial.println();
}
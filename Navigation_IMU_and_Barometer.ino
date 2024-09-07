/* ICARUS v1.0.4, Arduino, I2C
Combine IMU BNO055 with Barometer BME280, stageflight and displace all information at once
*/

//Library
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//BNO055 Initilization
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//BME280 Initillization
Adafruit_BME280 bme;

//Fire Button
const int Fire1pin = 2;
const int Fire2pin = 3;
int Fire1 = 0;
int FIre2 = 0;

//Temporary switch condition for apogee
const int ApogeePin = 4;
int Apogee = 0;

//environment measurement variable
#define SEALEVELPRESSURE_HPA (1013.25) // adding part it audo update


void setup() {
  Serial.begin(115200);
  Serial.println("ICARUS v1.0.4 - Navigation Measurement Test");


  //IMU Setup
  bool BNO_status = bno.begin();
  if (BNO_status) {
    Serial.println("BNO055 Detected");
  } else {
    Serial.println("Unable to Detect BNO055");
    while (1); // add something so it test all sensor because turn off
  }       
  bno.setExtCrystalUse(true); // switch to exteral crystal oscilator for more precision


  // Barometer Setup
   bool BME_status = bme.begin();
  if (BME_status) {
    Serial.println("Barometer BME280 Detected");
  } else {
    Serial.println("Unable to Detect Barometer BME280");
    while (1); 
  }       

  delay(1000);
  Serial.println();
}


void loop() {
  BME280serial();
  BNO055serial();
  delay(100);

}
void BNO055serial() {
  sensors_event_t event; //create instance for holding unfuse vector data
  imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER); //m/s^2
  imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE); // rps, radians per second
  imu::Vector<3> mag = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER); // uT, microTeslas

  bno.getEvent(&event); //get fused orientation data

  //acceleration data
  Serial.print("Accel X: ");
  Serial.print(accel.x(), 4);
  Serial.print("\tAccel Y: ");
  Serial.print(accel.y(), 4);
  Serial.print("\tAccel Z: ");
  Serial.println(accel.z(), 4);

  //gyro data
  Serial.print("Gyro X: ");
  Serial.print(gyro.x(), 4);
  Serial.print("\tGyro Y: ");
  Serial.print(gyro.y(), 4);
  Serial.print("\tGyro Z: ");
  Serial.println(gyro.z(), 4);

  //magneto data
  Serial.print("Mag X: ");
  Serial.print(mag.x(), 4);
  Serial.print("\tMag Y: ");
  Serial.print(mag.y(), 4);
  Serial.print("\tMag Z: ");
  Serial.println(mag.z(), 4);

  //orientation data (fused)
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");

  Serial.println(); //for one extra line space
}

void BME280serial() {
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();

}

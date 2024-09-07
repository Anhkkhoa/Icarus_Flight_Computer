/* ICARUS v1.0.3, Arduino, I2C
This code for test IMU BNO055, using I2C (do have SPI protocol)
*/

//library
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//BNO055 Initilization
Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  Serial.begin(115200);
  Serial.println("ICARUS v1.0.3 - BNO055 Testing");

  bool BNO_status = bno.begin();
  if (BNO_status) {
    Serial.println("BNO055 Detected");
  } else {
    Serial.println("Unable to Detect BNO055");
    while (1); 
  }       
  
  delay(1000); // waiting for loading after start up
  bno.setExtCrystalUse(true); // switch to exteral crystal oscilator for more precision

  Serial.println();
}

void loop() {
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
  delay(100);
}

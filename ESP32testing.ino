/* ICARUS v1.0.6
switched to ESP32
*/

//library
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <HardwareSerial.h>

//changing GPIO to TX and RX 
HardwareSerial mySerial(1);

//SPI Pin Initialization
#define BME_SCK 4
#define BME_MOSI 5
#define BME_MISO 7

//BME280 Initialization: SCK(SCL) - 4, MOSI(SDA) - 5,CS - 6, MISO(SDO) - 7
#define BME_CS 6
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

//BNO055 Initialization:  SDA - 9, SCL - 10,
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//Environmental Variable
#define SEALEVELPRESSURE_HPA (1013.25)
 
void setup() {
  //Serial Setup
  Serial.begin(115200);
  Serial.println("ICARUS v1.0.6");

  //Protocal & Wiring Setup
  Wire.begin(9, 10);

  //Barometer
  bool bme_status = bme.begin();
  if (bme_status) {
    Serial.println("Barometer BME280 detected");
  } else {
    Serial.println("Unable to detected Barometer BME280");
    while(1);
  }

  //IMU
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

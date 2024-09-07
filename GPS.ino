/* ICARUS v1.0.7
Testing gps, WORKED FUCKER
*/
#include <TinyGPSPlus.h>

// Create an instance of the TinyGPSPlus library
TinyGPSPlus gps;

void setup() {
  // Initialize the Serial Monitor for debugging
  Serial.begin(9600);        // For debugging output to Serial Monitor
  
  // Initialize Serial2 for communication with the GPS module
  Serial2.begin(9600, SERIAL_8N1, 15, 16);  // RX on GPIO 15, TX on GPIO 16
  
  // Allow time for GPS module to warm up and start acquiring satellites
  delay(3000);
}

void loop() {
  // Check if data is available from the GPS module
  while (Serial2.available() > 0) {
    char c = Serial2.read();
    // Feed the data to the TinyGPSPlus library
    if (gps.encode(c)) {
      displayInfo();  // Display GPS information
    }
  }
  
  // Check if we haven't received enough GPS data
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);  // Stop the program
  }
}

void displayInfo() {
  Serial.print(F("Location: "));
  
  // Check if GPS location data is valid
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);  // Print latitude with 6 decimal places
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);  // Print longitude with 6 decimal places
  } else {
    Serial.print(F("INVALID"));  // Indicate that the location data is invalid
  }
  
  Serial.println();
}

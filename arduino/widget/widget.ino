#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  delay(1000);  // Give serial time to initialize
  Serial.println("\nStarting...");
  
  EEPROM.begin(512); // Initialize EEPROM so we can read stored data
  delay(100);  // Give EEPROM time to initialize
  
  // Use this to erase the config if testing
  // ESP.eraseConfig();
  
  // If you want to write default creds to EEPROM, because you already know what they are
  // writeDefaultCreds();
  
  setupSerial();  // Initialize serial command interface
  printHelp();    // Show available commands
  
  setupWiFi(); // Connect to WiFi
}

void loop() {
  serialEvent();  // Check for serial commands
  processCommand();  // Process any received commands
}

#include <ESP8266WiFi.h>

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setupSerial() {
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void processCommand() {
  if (stringComplete) {
    inputString.trim();  // Remove any whitespace

    // Split command and value
    int spaceIndex = inputString.indexOf(' ');
    if (spaceIndex == -1) {
      // No space found, check for read or help command
      if (inputString == "read") {
        readCreds();
        Serial.println("\nCurrent credentials:");
        Serial.print("SSID: ");
        Serial.println(ap_name);
        Serial.print("Password: ");
        Serial.println("*****");
        Serial.print("Server URL: ");
        Serial.println(server_url);
        if (apikey.length() >= 4) {
          Serial.print("API Key: ");
          String shortKey = apikey.substring(0, 4);
          Serial.println(shortKey + "****");
        }
        Serial.print("Room Code: ");
        Serial.println(roomCode);
      } else if (inputString == "help") {
        printHelp();
      } else {
        Serial.println("Invalid command. Use: setssid, setpass, setserver, setapikey, read, or help");
      }
    } else {
      String command = inputString.substring(0, spaceIndex);
      String value = inputString.substring(spaceIndex + 1);

      if (command == "setssid") {
        writeEEPROM(0 * EEPROM_CHUNK_SIZE, value);
        ap_name = value;
        Serial.println("SSID updated");
      } else if (command == "setpass") {
        writeEEPROM(1 * EEPROM_CHUNK_SIZE, value);
        ap_pass = value;
        Serial.println("Password updated");
      } else if (command == "setserver") {
        writeEEPROM(2 * EEPROM_CHUNK_SIZE, value);
        server_url = value;
        Serial.println("Server URL updated");
      } else if (command == "setapikey") {
        writeEEPROM(3 * EEPROM_CHUNK_SIZE, value);
        apikey = value;
        Serial.println("API Key updated");
      } else if (command == "setroomcode") {
        writeEEPROM(4 * EEPROM_CHUNK_SIZE, value);
        roomCode = value;
        Serial.println("Room Code updated");
      } else {
        Serial.println("Invalid command. Use: setssid, setpass, setserver, setapikey, setroomcode, read, or help");
      }
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void printHelp() {
  Serial.println("\nAvailable commands:");
  Serial.println("setssid <ssid>     - Set WiFi SSID");
  Serial.println("setpass <password> - Set WiFi password");
  Serial.println("setserver <url>    - Set server URL");
  Serial.println("setapikey <key>    - Set API key");
  Serial.println("setroomcode <code> - Set API key");
  Serial.println("read               - Read all current values");
  Serial.println("help               - Show this help message");
}

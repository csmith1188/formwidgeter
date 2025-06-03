#include <EEPROM.h>

#define EEPROM_CHUNK_SIZE 64

String ap_name = "";
String ap_pass = "";
String server_url = "";
String apikey = "";
String roomCode = "";

void writeDefaultCreds() {
  Serial.println("\nWriting default credentials to EEPROM:");
  writeEEPROM(0 * EEPROM_CHUNK_SIZE, "THE WIFI SSID");
  writeEEPROM(1 * EEPROM_CHUNK_SIZE, "THE WIFI PASSWORD");
  writeEEPROM(2 * EEPROM_CHUNK_SIZE, "THE SERVER YOU WANT TO USE");
  writeEEPROM(3 * EEPROM_CHUNK_SIZE, "YOUR API KEY");
  writeEEPROM(4 * EEPROM_CHUNK_SIZE, "THE ROOM CODE");
  Serial.println("Finished writing default credentials");
}

void writeEEPROM(int address, const String &value) {
  // Serial.print("Writing to EEPROM at address ");
  // Serial.print(address);
  // Serial.print(": ");
  // Serial.println(value);

  // Erase EEPROM chunk before writing
  for (int i = address; i < address + EEPROM_CHUNK_SIZE; i++) {
    EEPROM.write(i, 0);
  }
  // Write new value
  for (unsigned int i = 0; i < value.length(); i++) {
    EEPROM.write(address + i, value[i]);
  }
  EEPROM.commit();

  // Verify the write
  // String verify = readEEPROM(address);
  // Serial.print("Verification read: ");
  // Serial.println(verify);
}

String readEEPROM(int startAddress) {
  String result = "";
  for (int i = startAddress; i < startAddress + EEPROM_CHUNK_SIZE; i++) {
    char c = (char)EEPROM.read(i);
    if (c != 0) {  // Only add non-null characters
      result += c;
    }
  }
  result.trim();  // Remove whitespace from both ends
  return result;
}

void readCreds() {
  // Serial.println("\nReading credentials from EEPROM:");
  for (byte i = 0; i < 5; i++) {
    String value = readEEPROM(i * EEPROM_CHUNK_SIZE);
    // Serial.print("Address ");
    // Serial.print(i * EEPROM_CHUNK_SIZE);
    // Serial.print(": ");
    // Serial.println(value);

    switch (i) {
      case 0:
        ap_name = value;
        break;
      case 1:
        ap_pass = value;
        break;
      case 2:
        server_url = value;
        break;
      case 3:
        apikey = value;
        break;
      case 4:
        roomCode = value;
        break;
      default:
        break;
    }
  }
  // Serial.println("Finished reading credentials");
}
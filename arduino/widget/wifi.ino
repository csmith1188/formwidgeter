#include <ESP8266WiFi.h>

#define WPS_TIMEOUT 30000

bool wifiConnected = false;

void showConnectionInfo() {
  Serial.println("\nConnected to WiFi!");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setupWiFi() {
  // Force WiFi mode to station
  // WiFi.mode(WIFI_STA);

  // Check if we already have WPS info stored
  if (WiFi.SSID().length() > 0) {
    Serial.println("Found stored credentials, attempting to reconnect...");
    WiFi.begin();  // Connect using stored credentials

    // Wait for connection
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      showConnectionInfo();
      wifiConnected = true;
      return;
    }
    Serial.println("\nFailed to connect with stored credentials");

  } else {
    connectWithCredentials();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nFailed to connect to WiFi with EEPROM credentials");
      Serial.println("Starting WPS...");

      // Begin WPS connection
      WiFi.beginWPSConfig();

      Serial.println("Waiting for WPS connection...");

      unsigned long startAttemptTime = millis();
      const unsigned long timeout = WPS_TIMEOUT;  // 30 seconds timeout

      // Wait until WiFi connects or timeout
      while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
        delay(500);
        Serial.print(".");
      }

      Serial.println();

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WPS Successful!");
        showConnectionInfo();
        wifiConnected = true;
      } else {
        Serial.println("WPS failed or timed out.");
      }
    }
  }
}

void connectWithCredentials() {
  Serial.println("Connecting to WiFi with EEPROM credentials...");
  // Read credentials from EEPROM first
  readCreds();
  // Check if we have credentials stored
  if (ap_name.length() == 0 || ap_pass.length() == 0) {
    Serial.println("No WiFi credentials found in EEPROM!");
    return;
  }

  WiFi.begin(ap_name.c_str(), ap_pass.c_str());

  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    showConnectionInfo();
    wifiConnected = true;
  } else {
    Serial.println("\nFailed to connect with EEPROM credentials!");
  }
}
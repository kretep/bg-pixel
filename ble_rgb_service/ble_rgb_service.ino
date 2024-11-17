#include <Adafruit_NeoPixel.h>
#include <ArduinoBLE.h>
#include "util.h" // Include the external utility

// NeoPixel setup
#define NEOPIXEL_PIN D0
#define NUM_PIXELS 1
Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// BLE setup
BLEService colorService("12345678-1234-1234-1234-123456789abc"); // Custom BLE service UUID
BLECharacteristic colorCharacteristic("87654321-4321-4321-4321-abcdefabcdef", BLEWrite, 3); // 3 bytes for HSV

// Store last HSV values
uint8_t lastHSV[3] = {0, 255, 51};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialize NeoPixel
  strip.begin();
  strip.show(); // Ensure NeoPixel is off at startup

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set up BLE service and characteristic
  BLE.setLocalName("NeoPixel Controller");
  BLE.setAdvertisedService(colorService);
  colorService.addCharacteristic(colorCharacteristic);
  BLE.addService(colorService);

  // Start advertising
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.println("Connected to central device");

    while (central.connected()) {
      // Check if the characteristic was written
      if (colorCharacteristic.written()) {
        // Get the HSV data (3 bytes)
        colorCharacteristic.readValue(lastHSV, 3);

        // Convert HSV to RGB
        uint8_t r, g, b;
        HSVtoRGB(lastHSV[0], lastHSV[1], lastHSV[2], r, g, b);

        // Debugging output
        Serial.print("HSV Received: ");
        Serial.print(lastHSV[0]);
        Serial.print(", ");
        Serial.print(lastHSV[1]);
        Serial.print(", ");
        Serial.println(lastHSV[2]);

        Serial.print("Converted RGB: ");
        Serial.print(r);
        Serial.print(", ");
        Serial.print(g);
        Serial.print(", ");
        Serial.println(b);

        // Update NeoPixel color
        strip.setPixelColor(0, strip.Color(r, g, b));
        strip.show();
      }

      delay(10); // Small delay to prevent excessive updates
    }

    Serial.println("Disconnected from central device");
  }
}

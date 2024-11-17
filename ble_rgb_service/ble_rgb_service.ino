#include <Adafruit_NeoPixel.h>
#include <ArduinoBLE.h>

// NeoPixel setup
#define NEOPIXEL_PIN D0
#define NUM_PIXELS 1
Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// BLE setup
BLEService colorService("12345678-1234-1234-1234-123456789abc"); // Custom BLE service UUID
BLECharacteristic colorCharacteristic("87654321-4321-4321-4321-abcdefabcdef", BLEWrite, 3); // 3 bytes for RGB

// Store last RGB values
uint8_t lastRGB[3] = {0, 0, 0}; // Default to off (black)

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
        // Get the RGB data (3 bytes)
        colorCharacteristic.readValue(lastRGB, 3);

        // Debugging output
        Serial.print("RGB Received: ");
        Serial.print(lastRGB[0]);
        Serial.print(", ");
        Serial.print(lastRGB[1]);
        Serial.print(", ");
        Serial.println(lastRGB[2]);
      }

      // Continuously update the NeoPixel with the last received color
      strip.setPixelColor(0, strip.Color(lastRGB[0], lastRGB[1], lastRGB[2]));
      strip.show();

      delay(10); // Small delay to prevent excessive updates
    }

    Serial.println("Disconnected from central device");
  }
}
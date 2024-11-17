#include "util.h"
#include <math.h> // Include for math functions like fmod

void HSVtoRGB(uint8_t h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b) {
  float hh = h / 255.0 * 360.0; // Scale hue to 0-360 degrees
  float ss = s / 255.0;         // Scale saturation to 0-1
  float vv = v / 255.0;         // Scale value to 0-1

  float c = vv * ss;            // Chroma
  float x = c * (1 - abs(fmod(hh / 60.0, 2) - 1));
  float m = vv - c;

  float r_prime, g_prime, b_prime;

  if (hh < 60) {
    r_prime = c; g_prime = x; b_prime = 0;
  } else if (hh < 120) {
    r_prime = x; g_prime = c; b_prime = 0;
  } else if (hh < 180) {
    r_prime = 0; g_prime = c; b_prime = x;
  } else if (hh < 240) {
    r_prime = 0; g_prime = x; b_prime = c;
  } else if (hh < 300) {
    r_prime = x; g_prime = 0; b_prime = c;
  } else {
    r_prime = c; g_prime = 0; b_prime = x;
  }

  r = (r_prime + m) * 255;
  g = (g_prime + m) * 255;
  b = (b_prime + m) * 255;
}

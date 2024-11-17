#ifndef UTIL_H
#define UTIL_H

#include <stdint.h> // Include for uint8_t

// Function to convert HSV to RGB
void HSVtoRGB(uint8_t h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b);

#endif

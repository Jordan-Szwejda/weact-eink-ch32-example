#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdint.h>
#include <algorithm>

using namespace std;

#define PROGMEM

#define pgm_read_word(p) (*(const uint16_t*)(p))
#define pgm_read_byte(addr) (*(const uint8_t*)(addr))

static bool isdigit(char ch)
{
  return ch>=48 && ch<=57;
}

#endif
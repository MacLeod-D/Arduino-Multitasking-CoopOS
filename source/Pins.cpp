// (C) 2019 Helmut Weber 

#include <Arduino.h>
#include <inttypes.h>

uint8_t *MyPinToPort (int pin) {
  if (pin<8) return &PIND;
  if ((pin>=8) & (pin<=13)) return &PINB;
}

uint8_t MyPinToBitMask (int pin) {
  if (pin<8) return (1<<pin);
  if ((pin>=8) & (pin<=13)) return (1<<(pin-8));
}

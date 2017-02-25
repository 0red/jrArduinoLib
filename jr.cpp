#include <Arduino.h>
#include "jr.h"

void jr_soft_reset() {
	asm volatile ("  jmp 0");
}

/*
void jr_i2c_detect(Stream *serial,Stream *wire) {
  jr_i2c_detect(0x03, 0x77,serial,wire);  // default range
}

void jr_i2c_detect(uint8_t first, uint8_t last,Stream *serial,Stream *wire) {
  uint8_t i, address, error;
  char buff[10];

  // table header
  serial.print("   ");
  for (i = 0; i < 16; i++) {
    //Serial.printf("%3x", i);
    sprintf(buff, "%3x", i);
    serial.print(buff);
  }

  // table body
  // addresses 0x00 through 0x77
  for (address = 0; address <= 119; address++) {
    if (address % 16 == 0) {
      //Serial.printf("\n%#02x:", address & 0xF0);
      sprintf(buff, "\n%02x:", address & 0xF0);
      serial.print(buff);
    }
    if (address >= first && address <= last) {
      wire.beginTransmission(address);
      error = wire.endTransmission();
      if (error == 0) {
        // device found
        //Serial.printf(" %02x", address);
        sprintf(buff, " %02x", address);
        serial.print(buff);
      } else if (error == 4) {
        // other error
        serial.print(" XX");
      } else {
        // error = 2: received NACK on transmit of address
        // error = 3: received NACK on transmit of data
        serial.print(" --");
      }
    } else {
      // address not scanned
      serial.print("   ");
    }
  }
  serial.println("\n");
}

*/

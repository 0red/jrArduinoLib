#include "jr.h"
#ifdef ESP8266
#include <Esp.h>
#endif


jrLed::jrLed(byte apin) {
  _previousMillis=millis();
  _interval=100;
  _state=LOW;
  _pin=apin;
  _setLed();
}

jrLed::jrLed(byte apin,byte ainterval) {
  _previousMillis=millis();
  _interval=ainterval;
  _state=LOW;
  _pin=apin;
  _setLed();
}

void jrLed::set (byte ainterval) {
  _interval=ainterval;
  _previousMillis=millis();
  if (_interval==LOW) {_state=LOW;}
  if (_interval==HIGH) {_state=HIGH;}
  _setLed();
}

byte jrLed::tick() {
	return tick(millis());
}

byte jrLed::tick (unsigned long currentMillis) {
	if (_interval==LOW || _interval==HIGH) return _state;
	if (currentMillis-_previousMillis>=(_interval*10)) {
		_state=(_state==LOW)?HIGH:LOW;
		_setLed();
	}
	return _state;
}

void jrLed::_setLed() {
  if (_pin>=0) {
		pinMode(_pin, OUTPUT); 
		digitalWrite(_pin,_state);
	}		
}

void jr_soft_reset() {
#ifdef __AVR__
  asm volatile ("  jmp 0");
#endif

#ifdef ESP8266
	ESP.reset();
#endif
}


int jrFreeRam () {
	//https://playground.arduino.cc/Code/AvailableMemory
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
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

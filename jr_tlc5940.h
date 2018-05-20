#ifndef JRtlc5940_h
#define JRtlc5940_h
#include "Arduino.h"

#define TLC5940_LED_MAX 2
#define TLC5940_LED_STEP 2
#define TLC5940_LED_TIMER 30
//int led_timer=30;
//byte led_step=5;

///#define NUM_TLCS  2 // in tlc_config.h !!!
#include "Tlc5940.h"


class JRtlc5940 {
 public:
	JRtlc5940(byte cs,byte ms); // cs - cable select, ms
	void led_loop();
	void led_set(byte led,byte value);
	int  demo_loop(int _delay);
 protected:
  byte _cs;
  byte _ms;
	byte led_state[TLC5940_LED_MAX*2]; 
	byte led_state_int[TLC5940_LED_MAX*16]; 
	unsigned long led_timer_int;
};
#endif


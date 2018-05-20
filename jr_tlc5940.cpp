#include <Arduino.h>
#include "jr.h"
#include "Tlc5940.h"
#include "jr_tlc5940.h"

//extern Tlc5940 Tlc;

JRtlc5940::JRtlc5940(byte cs,byte ms) {
	_cs=cs;
	_ms=ms?ms:TLC5940_LED_TIMER;
	led_timer_int=millis();
	Tlc.init(0); // Initiates the TLC5940 and set all channels off
  
  for (int i=0;i<TLC5940_LED_MAX*2;i++) { led_state[i]=0;}
  for (int i=0;i<TLC5940_LED_MAX*16;i++) {
        led_state_int[i]=255; 
        Tlc.set(i, 100);    
  }
  Tlc.update();
}

void JRtlc5940::led_loop() {
  bool update=false;
   if (led_timer_int+TLC5940_LED_TIMER<millis() || millis()<led_timer_int) {
      Serial.print(led_timer_int);Serial.print("+");
  Serial.print(TLC5940_LED_TIMER);Serial.print("=");
  Serial.print(millis());
 
  Serial.print("\n");
    for (int i=0;i<TLC5940_LED_MAX*16;i++) {
      if (GetBit(led_state,i)) {
        if (led_state_int[i]<255) {
          if (led_state_int[i]+TLC5940_LED_STEP>255) led_state_int[i]=255; else led_state_int[i]+=TLC5940_LED_STEP;
          update=true;
          Tlc.set(i, led_state_int[i] << 4);    
          Serial.print("  up:");Serial.print(i);Serial.print("=");Serial.print(led_state_int[i] << 4);Serial.print("\n");
        }
      } else {
        if (led_state_int[i]>0) {
          if (led_state_int[i]-TLC5940_LED_STEP<0) led_state_int[i]=0; else led_state_int[i]-=TLC5940_LED_STEP;
          update=true;
          Tlc.set(i, led_state_int[i] << 4);    
         Serial.print("down:");Serial.print(i);Serial.print("=");Serial.print(led_state_int[i] << 4);Serial.print("\n");
       }
      }
    }
    if (update)  Tlc.update(); // Activates the previously set outputs
    led_timer_int=millis();
  }
}

void JRtlc5940::led_set(byte pin,byte value) {
	led_state_int[pin]=value;
	Tlc.set(pin, led_state_int[pin] << 4);    
	Tlc.update(); // Activates the previously set outputs
}

int JRtlc5940::demo_loop(int _delay) {
  led_loop();
  if (led_state_int[0]==0 ) {
    delay(_delay);for (int i=0;i<TLC5940_LED_MAX*2;i++) { led_state[i]=B01010101;}
    return 1;
  }
  if (led_state_int[0]==255) {    
    delay(_delay);for (int i=0;i<TLC5940_LED_MAX*2;i++) { led_state[i]=B10101010;}
    return 2;
  }
  return 0;
}
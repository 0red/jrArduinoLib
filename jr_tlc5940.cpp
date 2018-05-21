#include <Arduino.h>
#define DEBUG

#include "jr.h"
#include "Tlc5940.h"
#include "jr_tlc5940.h"

extern Tlc5940 Tlc;

JRtlc5940::JRtlc5940(byte cs,byte ms) {
	_cs=cs;
	_ms=ms?ms:TLC5940_LED_TIMER;
	led_timer_int=millis();

}

void JRtlc5940::init() {
		
	Tlc.init(0); // Initiates the TLC5940 and set all channels off
  
  for (int i=0;i<TLC5940_LED_MAX*2;i++) { led_state[i]=0;}
  for (int i=0;i<TLC5940_LED_MAX*16;i++) {
        led_state_int[i]=255; 
        Tlc.set(i, 1000);    
  }
  Tlc.update();
}

void JRtlc5940::led_loop() {
  if (_ms==1) return;
  bool update=false;
   if (led_timer_int+TLC5940_LED_TIMER<millis() || millis()<led_timer_int) {
  //    Serial.print(led_timer_int);Serial.print("+");
  //Serial.print(TLC5940_LED_TIMER);Serial.print("=");
  //Serial.print(millis());
 
  //Serial.print("\n");
    for (int i=0;i<TLC5940_LED_MAX*16;i++) {
      if (GetBit(led_state,i)) {
        if (led_state_int[i]<255) {
          if (led_state_int[i]+TLC5940_LED_STEP>255) led_state_int[i]=255; else led_state_int[i]+=TLC5940_LED_STEP;
          update=true;
          Tlc.set(i, led_state_int[i] << 4);    
          //Serial.print("  up:");Serial.print(i);Serial.print("=");Serial.print(led_state_int[i] << 4);Serial.print("\n");
        }
      } else {
        if (led_state_int[i]>0) {
          if (led_state_int[i]-TLC5940_LED_STEP<0) led_state_int[i]=0; else led_state_int[i]-=TLC5940_LED_STEP;
          update=true;
          Tlc.set(i, led_state_int[i] << 4);    
          //Serial.print("down:");Serial.print(i);Serial.print("=");Serial.print(led_state_int[i] << 4);Serial.print("\n");
       }
      }
    }
    if (update)  Tlc.update(); // Activates the previously set outputs
    led_timer_int=millis();
  }
}

void JRtlc5940::led_set(byte pin,byte value) {
	if (pin>=TLC5940_LED_MAX*16) return;
	for (pin=0;pin<TLC5940_LED_MAX*16;pin++) {
		led_state_int[pin]=value;
		Tlc.set(pin, led_state_int[pin] << 4);    
	}
	Tlc.update(); // Activates the previously set outputs
}

void JRtlc5940::led_on(byte pin,byte value) {  
	if (pin>=TLC5940_LED_MAX*16) return;
	if (value) {SetBit(led_state,pin);} else {ClearBit(led_state,pin);}
}


int JRtlc5940::demo_loop(int _delay) {
  led_loop();
  
  if (led_state_int[0]==0 ) {
		JR_PRINTLNF("JRtlc5940::demo_loop1");
    delay(_delay);for (int i=0;i<TLC5940_LED_MAX*2;i++) { led_state[i]=B01010101;}
    return 1;
  }
  if (led_state_int[0]==255) {    
		JR_PRINTLNF("JRtlc5940::demo_loop2");
    delay(_delay);for (int i=0;i<TLC5940_LED_MAX*2;i++) { led_state[i]=B10101010;}
    return 2;
  }
  return 0;
}

void JRtlc5940::show(){
	#ifdef TLC5940_LED_MAX
		JR_PRINTLNF("JRtlc5940::show");
		for (int i=0;i<TLC5940_LED_MAX*16;i++) {
			JR_PRINTV("port",i);
			JR_PRINTV("int",led_state_int[i]<<4);
			JR_PRINTV("v",(GetBit(led_state,i))?LOW:HIGH);
			JR_LN;
		}	
		#endif 
		#ifndef TLC5940_LED_MAX
		JR_PRINTLNF("JRcd4051_cmdAll - TLC5940_LED_MAX not defined");
		#endif 
}

  extern JRtlc5940 jrtlc;
  
	bool JRtlc5940_cmdShow(ParserParam *p1) {
	//Serial.println("Loop JRtlc5940_cmdShow");
	JR_PRINTLNF("JRtlc5940_cmdShow");
		jrtlc.show();
	};
		
	bool JRtlc5940_cmdLed(ParserParam *p1) {
	#ifdef TLC5940_LED_MAX
		ParserParam p=*p1;
		if (p.i[1]>=0 && p.i[1]<TLC5940_LED_MAX*16) {
			JR_PRINTLNF("JRtlc59401_cmdLed");
			jrtlc.led_on(p.i[1],(p.i[2])?1:0);
		}
	#endif 
	#ifndef TLC5940_LED_MAX
		JR_PRINTLNF("JRtlc59401_cmdLed - TLC5940_LED_MAX not defined");
	#endif 	
	};
	
	bool JRtlc5940_cmdLedVal(ParserParam *p1) {
	#ifdef TLC5940_LED_MAX
		ParserParam p=*p1;
		if (p.i[1]>=0 && p.i[1]<TLC5940_LED_MAX*16 && p.i[2]<256) {
			JR_PRINTLNF("JRtlc59401_cmdLedVal");
			jrtlc.led_set(p.i[1],p.i[2]);
		}
	#endif 
	#ifndef TLC5940_LED_MAX
		JR_PRINTLNF("JRtlc59401_cmdLed - TLC5940_LED_MAX not defined");
	#endif 	
	};
	
		bool JRtlc5940_cmdDemo(ParserParam *p1) {
	#ifdef TLC5940_LED_MAX
		ParserParam p=*p1;
		jrtlc.demo_loop(1000);
	#endif 
	#ifndef TLC5940_LED_MAX
		JR_PRINTLNF("JRtlc59401_cmdLed - TLC5940_LED_MAX not defined");
	#endif 	
	};
	
	
	
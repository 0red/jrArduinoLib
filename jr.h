 /*
DebugUtils.h - Simple debugging utilities.
Copyright (C) 2011 Fabio Varesano <fabio at varesano dot net>

Ideas taken from:
http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1271517197

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/


/*
https://forum.arduino.cc/index.php?topic=50197.0

char *buffer;
buffer=(char *)malloc(strlen_P(progmemstring)+1);
strcpy_P(buffer,progmemstring);
Serial.println(buffer);
free(buffer);
*/

#ifndef jr_h
#define jr_h
#include "Arduino.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c %c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 
/*  
printf("Leading text "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(byte));
For multi-byte types

printf("m: "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n",
  BYTE_TO_BINARY(m>>8), BYTE_TO_BINARY(m));  
*/


#define SetBit(A,k)     ( A[((k)/8)] |= (1 << ((k)%8)) )
#define ClearBit(A,k)   ( A[((k)/8)] &= ~(1 << ((k)%8)) )            
#define TestBit(A,k)    ( A[((k)/8)] & (1 << ((k)%8)) )
#define GetBit(A,k)     ( A[((k)/8)] & (1 << ((k)%8)) )

#define GetBite(A,k)    ( A & (1 << ((k)%8)) )
#define SetBite(A,k)     ( A |= (1 << ((k)%8)) )
#define ClearBite(A,k)   ( A &= ~(1 << ((k)%8)) )            

#define get_byte(bits)   ((bits) >>3) + ( ((bits) % 8) >0 ) 

#define wire_writeword(res)   Wire.write(byte((res) & 0xFF)); Wire.write(byte((res) >>8 ));
#define wire_readword(res) 		res=Wire.read();res+=Wire.read()<<8;

class jrLed {
	public:
		jrLed(byte apin);
		jrLed(byte apin,byte ainterval);
		void set (byte ainterval);
		byte tick ();
		byte tick (unsigned long currentMillis);
	protected:
		unsigned long _previousMillis = 0;
		 byte  _interval=0;
		 byte  _pin=0;
		 byte  _state=0;
		void _setLed();
};

void jr_soft_reset();
int jrFreeRam ();
//https://playground.arduino.cc/Code/EEPROMWriteAnything

//void jr_i2c_detect(Stream *serial,Stream *wire);
//void jr_i2c_detect(uint8_t first, uint8_t last,Stream *serial,Stream *wire) ;

//http://www.boost.org/   BOOST_CURRENT_FUNCTION    https://msdn.microsoft.com/en-us/library/b0084kay(VS.80).aspx  //vs predefined macros
	

#ifdef DEBUG

 #define DEBUG_PRINT(str)    \
   Serial.print(millis());     \
   Serial.print(F(": "));    \
   Serial.print(__PRETTY_FUNCTION__); \
   Serial.print(F(" "));      \
   Serial.print(F(__FILE__));     \
   Serial.print(F(":"));      \
   Serial.print( __LINE__ );     \
   Serial.print(F(" "));      \
   Serial.println(str);

 #define JR_PRINT(x)     		Serial.print (x)
 #define JR_PRINTDEC(x)     Serial.print (x, DEC)
 #define JR_PRINTHEX(x)     Serial.print (x, HEX)
 #define JR_PRINTBIN(x)     Serial.print (x, BIN)
 #define JR_PRINTF(x)     	Serial.print (F(x))
 #define JR_PRINTLN(x)  		Serial.println (x)
 #define JR_PRINTLNF(x)  		Serial.println (F(x))
 
 #define JR_PRINTV(i,x)     		Serial.print (i);Serial.print (F("="));Serial.print (x);Serial.print (F(" "))
 #define JR_PRINTDECV(i,x)     Serial.print (i);Serial.print (F("="));Serial.print (x, DEC);Serial.print (F(" "))
 #define JR_PRINTHEXV(i,x)     Serial.print (i);Serial.print (F("="));Serial.print (x, HEX);Serial.print (F(" "))
 #define JR_PRINTBINV(i,x)     Serial.print (i);Serial.print (F("="));Serial.print (x, BIN);Serial.print (F(" "))
 #define JR_PRINTLNV(i,x)  		Serial.print (i);Serial.print (F("="));Serial.println (x)
 
 #define JR_V(x)	JR_PRINTV(#x,x)  
 #define JR_VD(x)	JR_PRINTDECV(#x,x)  
 #define JR_VH(x)	JR_PRINTHEXV(#x,x)  
 #define JR_VB(x)	JR_PRINTBINV(#x,x)  
 
 #define JR_VF(x)	JR_PRINTV(F(#x),x)  
 #define JR_VDF(x)	JR_PRINTDECV(F(#x),x)  
 #define JR_VHF(x)	JR_PRINTHEXV(F(#x),x)  
 #define JR_VBF(x)	JR_PRINTBINV(F(#x),x)   
 
 #define JR_LN		JR_PRINTLNF("")  
 #define JR_POINTER(slave)	JR_PRINTF ("["); JR_PRINTHEX((unsigned long) slave); JR_PRINTF ("]");
#else
 #define DEBUG_PRINT(str)
 #define JR_PRINT(x)     		
 #define JR_PRINTF(x)     	
 #define JR_PRINTDEC(x)     
 #define JR_PRINTHEX(x)     
 #define JR_PRINTBIN(x)     
 #define JR_PRINTLN(x)  		
 #define JR_PRINTLNF(x)  		
 
 #define JR_PRINTV(i,x)     		
 #define JR_PRINTDECV(i,x)     
 #define JR_PRINTHEXV(i,x)     
 #define JR_PRINTBINV(i,x)     
 #define JR_PRINTLNV(i,x)  		
 
 #define JR_V(x)
 #define JR_VD(x)
 #define JR_VH(x)
 #define JR_VB(x)
 
 #define JR_VF(x)
 #define JR_VDF(x)
 #define JR_VHF(x)
 #define JR_VBF(x)
 
 #define JR_LN
 #define JR_POINTER(slave)
#endif


#endif	



/*
Wire.begin (MY_ADDRESS);  // initialize hardware registers etc.
  TWAR = (MY_ADDRESS << 1) | 1;  // enable broadcasts to be received
  Wire.onReceive(receiveEvent);  // set up receive handler
  http://www.gammon.com.au/i2c
  
  https://github.com/marcobrianza/ClickButton
  https://github.com/sstaub/Ticker
  https://github.com/esp8266/Arduino/tree/master/libraries/Ticker
  https://github.com/PRosenb/DeepSleepScheduler
  https://tttapa.github.io/ESP8266/Chap07%20-%20Wi-Fi%20Connections.html
  https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266mDNS
  https://tttapa.github.io/ESP8266/Chap08%20-%20mDNS.html
  https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/IPAddress.cpp#L103-L113
*/
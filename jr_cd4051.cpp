#include <Arduino.h>
#include "jr.h"
#include "jr_cd4051.h"


JRcd4051::JRcd4051(int outpin,int a,int b,int c) {
	_outpin=outpin;
	_a=a;
	_b=b;
	_c=c;
	pinMode(a,OUTPUT);
	pinMode(b,OUTPUT);
	pinMode(c,OUTPUT);
}



void JRcd4051::_set(int port) {
	byte _port[1];
	_port[0]=port % 8;
	digitalWrite(_a, (GetBit(_port,0)?HIGH:LOW));	
	digitalWrite(_b, (GetBit(_port,1)?HIGH:LOW));
	digitalWrite(_c, (GetBit(_port,2)?HIGH:LOW));
}

int JRcd4051::getD(int port,int treshold){
  return (getA(port)>=treshold)?HIGH:LOW;
}

int JRcd4051::getA(int port){
	_set(port);
	return analogRead(_outpin);
}




#ifdef CD4051_JR_BOARD
	int JRcd4051_analog[16];
	int JRcd4051_treshold[16];
	byte JRcd4051_digital[2];
	
	void JRcd4051_readAll(){
		JRcd4051_digital[0]=0;JRcd4051_digital[1]=0;
		for (int i=0;i<8;i++) {
		#ifdef CD4051_KPIN
		JRcd4051_analog[i]=JRcd4051k.getA(i);
		if (JRcd4051_analog[i]>=JRcd4051_treshold[i]) SetBit(JRcd4051_digital,i);
			#ifdef CD4051_LPIN
				JRcd4051_analog[i+8]=analogRead(CD4051_LPIN);
			if (JRcd4051_analog[i+8]>=JRcd4051_treshold[i+8]) SetBit(JRcd4051_digital,i+8);
			#endif 		
		#endif
	  #ifndef CD4051_KPIN
				JRcd4051_analog[i+8]=JRcd4051l.getA(i);
			if (JRcd4051_analog[i+8]>=JRcd4051_treshold[i+8]) SetBit(JRcd4051_digital,i+8);
		#endif 		
		}
	}
	
	
	#ifdef CD4051_KPIN
		JRcd4051 JRcd4051k(CD4051_KPIN,CD4051_APIN,CD4051_BPIN,CD4051_CPIN);
	#endif
	#ifdef CD4051_LPIN
		JRcd4051 JRcd4051l(CD4051_LPIN,CD4051_APIN,CD4051_BPIN,CD4051_CPIN);
	#endif 
#endif

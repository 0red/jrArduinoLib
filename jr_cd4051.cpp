#include <Arduino.h>
#define DEBUG
#define TRESHOLD_BELOW 1


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



void JRcd4051::_set(byte port) {
//	byte _port[1];
//	_port[0]=port % 8;
	port=port % 8;
	digitalWrite(_a, (GetBite(port,0)?HIGH:LOW));	
	digitalWrite(_b, (GetBite(port,1)?HIGH:LOW));
	digitalWrite(_c, (GetBite(port,2)?HIGH:LOW));
}

int JRcd4051::getD(int port,int treshold){
#ifdef TRESHOLD_BELOW
  return (getA(port)<=treshold)?HIGH:LOW;
#endif
#ifndef TRESHOLD_BELOW
  return (getA(port)>=treshold)?HIGH:LOW;
#endif
  
}

int JRcd4051::getA(int port){
	if (port==16) analogRead(A0);
	_set(port);
	return analogRead(_outpin);
}




#ifdef CD4051_JR_BOARD
	int JRcd4051_analog[17];
	int JRcd4051_treshold[17]={500,500,500,500,500,500,500,500,500,500,500,500,500,500,500,500,500};
	byte JRcd4051_digital[3];
	
	void JRcd4051_readAll(){
		JRcd4051_digital[0]=0;JRcd4051_digital[1]=0;JRcd4051_digital[2]=0;
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
		JRcd4051_analog[16]=analogRead(A0);
		if (JRcd4051_analog[16]>=JRcd4051_treshold[16]) SetBit(JRcd4051_digital,16);
		
	}
	
	
	#ifdef CD4051_KPIN
		JRcd4051 JRcd4051k(CD4051_KPIN,CD4051_APIN,CD4051_BPIN,CD4051_CPIN);
	#endif
	#ifdef CD4051_LPIN
		JRcd4051 JRcd4051l(CD4051_LPIN,CD4051_APIN,CD4051_BPIN,CD4051_CPIN);
	#endif 
#endif


	bool JRcd4051_cmdAll(ParserParam *p1) {
	#ifdef CD4051_JR_BOARD
		JR_PRINTLNF("JRcd4051_cmdAll");
		JRcd4051_readAll();
		JR_PRINTLNF("JRcd4051_readAll");
		for (int i=0;i<17;i++) {
			JR_PRINTV("port",i);
			JR_PRINTV(" analog",JRcd4051_analog[i]);
			JR_PRINTV(" treshold",JRcd4051_treshold[i]);
			JR_PRINTV(" digital",(GetBit(JRcd4051_digital,i))?LOW:HIGH);
			JR_LN;
		}	
		#endif 
		#ifndef CD4051_JR_BOARD
		JR_PRINTLNF("JRcd4051_cmdAll - CD4051_JR_BOARD not defined");
		#endif 
	};
		
	bool JRcd4051_getA(ParserParam *p1) {
	#ifdef CD4051_JR_BOARD
		JR_PRINTLNF("JRcd4051_getA");
		JRcd4051_readAll();
		JR_PRINTLNF("JRcd4051_readAll");
		ParserParam p=*p1;
		if (p.i[1]<16) {
			JR_PRINTV(" read port",p.i[1]);
			JR_PRINTV("analog",JRcd4051_analog[p.i[1]]);
			JR_PRINTV(" treshold",JRcd4051_treshold[p.i[1]]);
			JR_PRINTV(" digital",(GetBit(JRcd4051_digital,p.i[1]))?LOW:HIGH);
			JR_LN;
		}
	#endif 
	#ifndef CD4051_JR_BOARD
		JR_PRINTLNF("JRcd4051_getA - CD4051_JR_BOARD not defined");
	#endif 	
	};

	bool JRcd4051_setTreshhold(ParserParam *p1) {
	#ifdef CD4051_JR_BOARD
		JR_PRINTLNF("JRcd4051_setTreshhold");
		ParserParam p=*p1;
		if (p.i[1]<16 && p.i[2]<1024 ) {
			JRcd4051_treshold[p.i[1]]=p.i[2];
			JR_PRINTV("port",p.i[1]);
			JR_PRINTV("treshold",p.i[2]);
			JR_LN;
		}
	#endif 
	#ifndef CD4051_JR_BOARD
		JR_PRINTLNF("JRcd4051_getA - CD4051_JR_BOARD not defined");
	#endif 	
 	};
	
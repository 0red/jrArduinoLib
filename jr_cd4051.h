#ifndef jr_cd4051_h
#define jr_cd4051_h
#include "Arduino.h"
#include "jrCMD.h"

#define CD4051_APIN A1
#define CD4051_BPIN A2
#define CD4051_CPIN A3
#define CD4051_KPIN A6
#define CD4051_LPIN A7
#define CD4051_JR_BOARD 1 


//int led_timer=30;
//byte led_step=5;



class JRcd4051 {
	public:
		JRcd4051(int outpin,int a,int b,int c); // cs - cable select, a,b,c - byte
		int getD(int port,int treshold);
		int getA(int port);
//	int  demo_loop(int _delay);
	protected:
		byte _a;
		byte _b;
		byte _c;
		byte _outpin;  
		void _set(byte port);
};

	#ifdef CD4051_JR_BOARD
	extern int JRcd4051_analog[17];
	extern int JRcd4051_treshold[17];
	extern byte JRcd4051_digital[3];
	extern JRcd4051 JRcd4051k,JRcd4051l;
		
	void JRcd4051_readAll();
	bool JRcd4051_cmdAll(ParserParam *p1);		
	bool JRcd4051_getA(ParserParam *p1);		
	bool JRcd4051_setTreshhold(ParserParam *p1);	
	#endif

#endif


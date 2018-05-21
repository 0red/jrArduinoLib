#ifndef JRCMD_h
#define JRCMD_h

#ifndef CMD_FOO 
#define CMD_FOO 7
#endif

#ifndef CMD_PARAMS
#define CMD_PARAMS 5
#endif

#ifndef CMD_BUF
//#define CMD_BUF (1+(CMD_FOO+1)*CMD_PARAMS)
#define CMD_BUF 32
#endif

#ifdef __AVR__
#include <avr/wdt.h>
#endif
#include <stdint.h>
#include <string.h>

#include <Arduino.h>

//not handle the del & backspace !!!!!!!!!!!!!

struct ParserParam {
  char c[CMD_PARAMS][CMD_FOO+1];			//command [0] and params [1..CMD_PARAMS]
  int  i[CMD_PARAMS];									//itoa(command[i]
  char raw[CMD_BUF+1];									//raw command buffer
};

typedef bool jrCommandFoo(ParserParam *p);   //abstract function which handle the command


struct ParserCmd {
  const char *command;
  jrCommandFoo *foo;
  boolean progmem;
};


class JRcmd {
 public:
//	typedef bool jrCommandFoo(ParserParam *p);
	JRcmd ();
	
	int  proceed(Stream *serial);  //parse from steam
	int  parse(char *bufor,byte bufor_used); // parse only from buffer
	int  add (char const *cmd,jrCommandFoo *_foo); //add command
	int  add (const __FlashStringHelper* cmd,jrCommandFoo *_foo);//add command with F() PROGMEM
	int  standard(); //add command default;
	
 private:
  int parserCmdCount=0;
  ParserCmd *parserCmd;
};
	
	bool jr_cmd_pin  (ParserParam *p1);				// set dig pin to 0/1
	bool jr_cmd_show (ParserParam *p1);				// for showing A0/A7 in real on the Board
	bool jr_cmd_reset(ParserParam *p1);				// jump(0) - finally for reset board --> watchdog problem !!! avr/wdt
	bool jr_cmd_softreset(ParserParam *p1);		// jump(0) - soft reset -
	bool jr_cmd_getA (ParserParam *p1);				// get value of Analog PIN
	bool jr_cmd_getD (ParserParam *p1);				// get value of Digital PIN
	bool jr_cmd_main (ParserParam *p1);				// get value of pins
	bool jr_cmd_clean(ParserParam *p1);				// clean monitor (20 Line Feed)
	
#endif

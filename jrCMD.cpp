#define DEBUG

//#define ARDUINO_MEGA 

#include "jrCMD.h"
#include "jr_dynarray.h"
#include "jr.h"
#include <string.h>

#ifdef __AVR__
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#endif

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

/*
byte commandCount=0;
ParserCmd *Commands;


struct ParserParam {
  char c[CMD_PARAMS][CMD_FOO+1];
  int  i[CMD_PARAMS];
};

typedef bool jrCommandFoo(ParserParam *p);


struct ParserCmd {
  char command[CMD_FOO+1];
  jrCommandFoo *foo;
};



*/

int JRcmd::add(char const *cmd,jrCommandFoo *_foo) {
	ParserCmd newItem;
	
	JR_PRINTF("JRcmd::add  ");JR_POINTER(cmd); JR_PRINTF(" ");JR_PRINTLN(cmd); 	
	//strcpy(newItem.command,cmd);
	newItem.command=cmd;
	newItem.foo=_foo;
	newItem.progmem=false;
	jrDYN;jrDYN_Add(parserCmd,newItem,parserCmdCount,sizeof(ParserCmd));
	return parserCmdCount;
}	
/*
int JRcmd::addF(char const *cmd,jrCommandFoo *_foo) {
	ParserCmd newItem;
	
	strcpy_P(newItem.command,cmd);
	JR_PRINTF("JRcmd::addF ");JR_POINTER(cmd); JR_PRINTF(" ");JR_PRINTLN(newItem.command); 	
	//newItem.command=*cmd;
	newItem.foo=_foo;
	jrDYN;jrDYN_Add(parserCmd,newItem,parserCmdCount,sizeof(ParserCmd));
	return parserCmdCount;
}	
*/

int JRcmd::add(const __FlashStringHelper* cmd,jrCommandFoo *_foo) {
	ParserCmd newItem;
	//strcpy_P(newItem.command,(const char*) cmd);
	newItem.command=(const char*) cmd;
	newItem.foo=_foo;
	newItem.progmem=true;
	jrDYN;jrDYN_Add(parserCmd,newItem,parserCmdCount,sizeof(ParserCmd));
	JR_PRINTF("JRcmd::addF ");JR_POINTER(cmd); JR_PRINTF(" ");JR_PRINTLN(cmd); 	
}

//__FlashStringHelper*

// --------------------------- default commands ------------------------
/*
bool cmd_pin2(ParserParam *p1) {
	JR_PRINTLNF("cmd_pin2"); 
  ParserParam p=*p1;
	digitalWrite(p.i[1],(p.i[2]==0)?LOW:HIGH);
	JR_PRINTV("port",p.i[1]);
	JR_PRINTV("val",(p.i[2]==0)?LOW:HIGH);
	JR_LN;
};
*/

	
int  JRcmd::standard() {
  JR_PRINTLNF("JRcmd::standard"); 
  
	add(F("MAIN") ,&jr_cmd_main);
	add(F("PIN")  ,&jr_cmd_pin); 
//	add(F("PIN2") ,&cmd_pin2); 
	add(F("GETA") ,&jr_cmd_getA);
	add(F("GETD") ,&jr_cmd_getD);
	add(F("SHOW") ,&jr_cmd_show);
	add(F("RESET"),&jr_cmd_reset);
	add(F("SOFT") ,&jr_cmd_softreset);
	add(F("CLEAN"),&jr_cmd_clean);
	
	return parserCmdCount;

}



bool jr_cmd_pin(ParserParam *p1) {
  JR_PRINTLNF("jr_cmd_pin"); 
  ParserParam p=*p1;
	digitalWrite(p.i[1],(p.i[2]==0)?LOW:HIGH);
	JR_PRINTV("port",p.i[1]);
	JR_PRINTV("val",(p.i[2]==0)?LOW:HIGH);
	JR_LN;
};

bool jr_cmd_show(ParserParam *p1) {
//bool jr_cmd_show(ParserParam *p1){
  JR_PRINTLNF("jr_cmd_show"); 
  ParserParam p=*p1;
	JR_PRINTV(F("LOW"),LOW);
	JR_PRINTV(F("HIGH"),HIGH);	
	#ifdef __AVR__
	JR_PRINTV(F("A0"),A0);	
	JR_PRINTV(F("A7"),A7);		
	#endif
	#ifdef ESP8266
	#endif
	JR_PRINTV(F("CMD_BUF"),CMD_BUF);
	JR_LN;
};

bool jr_cmd_reset(ParserParam *p1) {
//bool jr_cmd_reset(ParserParam *p1){
  JR_PRINTLNF("jr_cmd_reset"); 
  ParserParam p=*p1;
  
	//wdt_enable(WDTO_8S); // https://ariverpad.wordpress.com/2012/02/26/resetting-the-arduino-through-software-for-fun-and-profit/
	//https://www.arduino.cc/en/Hacking/Bootloader  https://www.forums.adafruit.com/viewtopic.php?f=8&t=15435
#ifndef ESP8266
#endif
};

bool jr_cmd_softreset(ParserParam *p1) {
	JR_PRINTLNF("jr_cmd_softreset"); 
};


bool jr_cmd_getA(ParserParam *p1){
//bool jr_cmd_getA(ParserParam *p1){
	JR_PRINTLNF("jr_cmd_getA"); 
  ParserParam p=*p1;
  if (p.i[1]<14) {p.i[1]+=A0;}
	JR_PRINTV(p.i[1],analogRead(p.i[1]));
};

bool jr_cmd_getD(ParserParam *p1) {
//bool jr_cmd_getD(ParserParam *p1){
	JR_PRINTLNF("jr_cmd_getD"); 
  ParserParam p=*p1;
	JR_PRINTV(p.i[1],digitalRead(p.i[1]));
};

bool jr_cmd_clean(ParserParam *p1) {
//bool jr_cmd_getD(ParserParam *p1){
	JR_PRINTLNF("jr_cmd_clean"); 
  ParserParam p=*p1;
	if (!p.i[1]) p.i[1]=20;
	for (int i=0;i<p.i[1];i++) { JR_LN;} 
	
};



bool jr_cmd_main(ParserParam *p1){
	JR_PRINTLNF("jr_cmd_main"); 
  ParserParam p=*p1;
	 JR_LN; JR_PRINTF("A: ");
		
  for (int i=0;i<=8;i++) {
		JR_PRINTV(i,analogRead(i+A0));		
		//if (i%5==3) { JR_LN;}
  }
  JR_LN;

  #ifdef ARDUINO_MEGA 
  for (int i=0;i<54;i++) {
	#endif
  #ifndef ARDUINO_MEGA 
  for (int i=0;i<20;i++) {
  #endif
		if (i%10==0) { JR_LN; JR_PRINTF("0: ");}
		if (i<10) { JR_PRINTF(" ");}
		JR_PRINTV(i,digitalRead(i));		
  }
  JR_LN;JR_LN;

  
};


// --------------------------- parse ------------------------

int JRcmd::parse(char *bufor,byte bufor_used){

      ParserParam cmd;

  if (bufor == NULL || bufor_used>=CMD_BUF) {
        return -1;
    }

      //clear cmd
      for (int l=0;l<CMD_PARAMS;l++) {
        for (int k=0;k<CMD_FOO+1;k++) {
          cmd.c[l][k]=0;
        }
        cmd.i[l]=0;
      }
			for (int k=0;k<CMD_BUF+1;k++) {
				cmd.raw[k]=0;
			}
			
      int k=0;int j=0;
      bufor[bufor_used]=' '; // for processing last params
      {for (int i=0;i<bufor_used+1;i++) {
				if (j>=CMD_PARAMS) break;
        if (bufor[i]==' ' || k>=CMD_FOO) {
          cmd.c[j][k]=0;
          cmd.i[j]=atoi(cmd.c[j]);
          k=0;j++;          
        } else {
          cmd.c[j][k++]=bufor[i];
          cmd.c[j][k]=0;     
        }
       };
      }
      
      bufor[bufor_used]=0;
      JR_PRINTLN(F("Receive Command:"));//
      JR_PRINT(F("POS INT STR : "));
      JR_PRINTLN(bufor);
      for (int l=0;l<CMD_PARAMS;l++) {
         JR_PRINTDEC(l);JR_PRINTF("   ");
         JR_PRINTDEC(cmd.i[l]);JR_PRINTF("   ");
         JR_PRINTLN(cmd.c[l]);
       }
       
       
			//find parserCmd
			JR_PRINTLN(F("ParserCmd"));
			for (int l=0;l<parserCmdCount;l++) {
				//JR_VF(l);JR_PRINT(cmd.c[0]);JR_PRINTF("-");JR_PRINT(parserCmd[l].command);JR_LN;
				if  ((!parserCmd[l].progmem && strcmp(cmd.c[0],parserCmd[l].command)==0) 
						||
						(parserCmd[l].progmem && strcmp_P(cmd.c[0],parserCmd[l].command)==0)) {
						strcpy(cmd.raw,bufor);
						parserCmd[l].foo(&cmd);
						return l;
				}
			}
   return -2;
}


JRcmd::JRcmd() {
;
}

// --------------------------- Stream proceed ------------------------
int JRcmd::proceed(Stream *serial){
  
  static char bufor[CMD_BUF]="";
      // UART initialize?
  static byte bufor_pos=0;


//DEBUG_PRINT();

  if (serial == NULL) {
        return false;
    }

    
  while (serial->available()>0 && bufor_pos<CMD_BUF) {
    char c=serial->read();
//    JR_VD(bufor_pos);JR_VD(c);

    if ((c>='0' && c<='9') || (c>='A' && c<='Z')) {      
       bufor[bufor_pos++]=c; 
       bufor[bufor_pos]=0; 
       continue;
    }
    
    if ((c>='a' && c<='z')) { 
      bufor[bufor_pos++]=c-32; 
      continue;
    }
    
    if (bufor_pos && c==' ' && bufor[bufor_pos-1]!=' ') {    
      bufor[bufor_pos++]=c; 
//      bufor_pos++;     
      continue;   
    }
//    JR_LN;
    if (byte(c)==10 || byte(c)==13 || byte(c)==0) {
    
			if (parse(bufor,bufor_pos)<0) {
				//no command found (yet)
				//print help with avaiable registered function 
				JR_PRINTF("Available functions:");
				for (int l=0;l<parserCmdCount;l++) {
				 JR_PRINTF(" ");
				 if (parserCmd[l].progmem) JR_PRINT((const __FlashStringHelper*) parserCmd[l].command); 
															else JR_PRINT(parserCmd[l].command);
				 }
				 JR_LN;
			}
       //clear buffer
       bufor_pos=0;
       bufor[0]=0;
       for (int l=0;l<CMD_BUF;l++) {
        bufor[l]=0;
       }
       
    } 
  } 
}

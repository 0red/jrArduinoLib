
#ifndef JRCMD_h
#define JRCMD_h

#ifndef CMD_FOO 
#define CMD_FOO 7
#endif

#ifndef CMD_PARAMS
#define CMD_PARAMS 4
#endif

#ifndef CMD_BUF
#define CMD_BUF (1+(CMD_FOO+1)*CMD_PARAMS)
#endif

#include <avr/wdt.h>
#include <stdint.h>
#include <string.h>

#include <Arduino.h>


struct ParserParam {
  char c[CMD_PARAMS][CMD_FOO+1];
  int  i[CMD_PARAMS];
  char raw[CMD_BUF];
};

typedef bool jrCommandFoo(ParserParam *p);


struct ParserCmd {
  char command[CMD_FOO+1];
  jrCommandFoo *foo;
};



class JRcmd {
 public:
//	typedef bool jrCommandFoo(ParserParam *p);
	JRcmd ();
	
	int  proceed(Stream *serial);  //parse from steam
	int  parse(char *bufor,byte bufor_used); // parse only from buffer
	int  add (char const *cmd,jrCommandFoo *_foo); //add command
	int  addF(char const *cmd,jrCommandFoo *_foo);
	int  add(const __FlashStringHelper* cmd,jrCommandFoo *_foo);
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
	
 
/*

bool jr_cmd_rr(ParserParam *p1);

bool jr_cmd_wr(ParserParam *p1);


ParserCmd parserCmd[]={
	{"STATE",&jr_cmd_state},
	{"PIN",&jr_cmd_pin},
	{"SPI",&jr_cmd_spi},
	{"GETA",&jr_cmd_getA},
	{"GETD",&jr_cmd_getD},
	{"SHOW",&jr_cmd_show},
	{"RESET",&jr_cmd_reset},
	{"UPDATE",&jr_cmd_update},
	{"RR",&jr_cmd_rr},
	{"WR",&jr_cmd_wr},
};


int jr_command_parse(char *bufor,byte bufor_used) {
      ParserParam cmd;

  if (bufor == NULL || bufor_used>=CMD_BUF) {
        return -1;
    }


      //clear cmd
      int k;
      for (int l=0;l<CMD_PARAMS;l++) {
        for (k=0;k<CMD_FOO;k++) {
          cmd.c[l][k]=0;
        }
        cmd.i[l]=0;
      }

      
      k=0;int j=0;
      bufor[bufor_used++]=' '; // for processing last params
      {for (int i=0;i<bufor_used;i++) {
        if (bufor[i]==' ') {
          cmd.c[j][k]=0;
          cmd.i[j]=atoi(cmd.c[j]);
          k=0;j++;          
        } else {
          cmd.c[j][k++]=bufor[i];
          cmd.c[j][k]=0;     
        }
       };
      }
      
      JR_PRINTLN(F("Receive Command:"));
      JR_PRINTLN(F("POS INT STR"));
      for (int l=0;l<CMD_PARAMS;l++) {
         JR_PRINTDEC(l);JR_PRINTF(" ");
         JR_PRINTDEC(cmd.i[l]);JR_PRINTF(" ");
         JR_PRINTLN(cmd.c[l]);
       }
       
       
			//find parserCmd
			JR_PRINTLN(F("ParserCmd"));
			for (int l=0;l<sizeof(parserCmd)/sizeof(ParserCmd);l++) {
				if  (strcmp(cmd.c[0],parserCmd[l].command)==0) {
						parserCmd[l].foo(&cmd);
						return l;
				}
			}
   return -2;
}

int jr_command(Stream *serial) {
  
  static char bufor[CMD_BUF]="";
      // UART initialize?
  static byte bufor_pos=0;

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
    
			if (jr_command_parse(bufor,bufor_pos)<0) {
				//no command found
				//print help
				JR_PRINTF("Available functions:");
				for (int l=0;l<sizeof(parserCmd)/sizeof(ParserCmd);l++) {
				 JR_PRINTF(" ");
				 JR_PRINT(parserCmd[l].command);
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
*/

#endif
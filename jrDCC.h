#ifndef JRDCC_h
#define JRDCC_h

// if You use more than A7 needs to chage to 4
// 4 max 16 Analog ports (Mega) 3 max A7 (Nano)
#define ANALOG_MAX 		4
// 6 max 64 Digital ports (Mega) 4 max A7 (Nano)
#define DIGITAL_MAX 	5
// max allowed module and track length
#define LED_MAX 	5
// max allowed module and track length
#define NAME_MAX 			10

// how many bits will be 
#define SENSORS_LEN	126
#define OCCUPANCYS_LEN	48
#define SIGNALS_LEN	58
#define RELAYS_LEN	37
#define TOTAL_LEN		SENSORS_LEN+OCCUPANCYS_LEN+RELAYS_LEN+SIGNALS_LEN*SIGNAL_TYPE_LEN
#define NRSEN_LEN	7
#define NRSIG_LEN	7
#define SIGNAL_TYPE_LEN 3




#define SIZE_LOCAL ANALOG_LOCAL+DIGITAL_LOCAL+LED_MAX+1
// +1 for bit informing for pending message to MASTER

#define PROTOCOL_VER 	1

#define jrD0 	0
#define jrD1 	1
#define jrD2 	2
#define jrD3 	3
#define jrD4 	4
#define jrD5 	5
#define jrD6 	6
#define jrD7 	7
#define jrD8 	8
#define jrD9 	9
#define jrD14 	14
#define jrD15 	15
#define jrD16 	16
#define jrD17 	10
#define jrD18 	11
#define jrD19 	12
#define jrD20 	13
#define jrCS	10
#define jrINT	2







// get number of requested bytes based on bits number
//int  get_byte(int bits);

//#define SIZE_LOCAL ANALOG_LOCAL+DIGITAL_LOCAL
//#define BYTE_LOCAL (SIZE_LOCAL >> 3) + (((SIZE_LOCAL-0) % 8)>0) 


// -------------------------------------- Arduino State ------------------------------

enum ardState {
  // block DCC current (if on board) and initialize module
  initial=0  ,         //0
  // error code
  // i.e. incopatible PROTOCOL_VER
  error_state,
  //check if connection still exists
  keep_alive,		//2
  //send Arduino board info and reset quere
  hand_shake,
  //reset Pin quere and send first Digital
  reset_pin_info,		//4
  // send NEXT Digial Pin and after Analog Pin or NULL if no more Pin Info
  get_pin_info, 
  // send NEXT Digial Pin info or NULL if no more Digital Pin Info
  // get all states bits (0 no trigger, 1 triggers occupied on) based on the Digital+Analog info order
  get_states,		//6
  // pin --> <A0 analog array >=A0(14 HW number)
  // return 2bytes Analog value of the PIN + 2bytes treshhold (total 4bytes)
  get_Apin,
  // set the Pin state -->0,1
  // spi (0..3 port 7bit=1 (128) --> according to digital table 
  // port (spi-0..3 - real port SPI>127 --> position in digital table
  // val LOW(0) HIGH(~0)
  setD,		//8
  // set the threshold
  set_Atreshold,
  // receive message from I2C Master - 1 byte length (<=CMD_BUF) - over CMD_BUF will be read but ignored
  msg_to_slave,	//10
  // sending message to I2C Master - ALWAYS CMD_BUF bytes
  msg_to_master,
  // sending block msg
  block_msg,
  
};


//#define MIN(a,b) (((a)<(b))?(a):(b))
//#define MAX(a,b) (((a)>(b))?(a):(b))

/*
const byte ardStateLen[]={
	0, //initial
	0, //keep_alive
	1+sizeof(ArduinoU), //hand_shake ProtocolVer+Arduino
	2, //reset_pin_info //digital,analog
	sizeof(DigitalPinU),//get_pin_info
	//get_states
	//get_Apin/
	0,//setD
	0,//set_Atreshold
};
*/
	
// -------------------------------------- dccType ------------------------------
// type of DCC connected devices

enum dccType {
	//0-3 analog/digital
	//0-15 digital
  no_dcc=0  ,         // 0
  proximity,					// 1 (analog only) zblizeniowy
  analogSPARE1,				// 2
  analogSPARE2,				// 3
  spare,							// 4	//used in case of additional bit is needed for state forward switch3,xswitch,sig3
  light,              // 5
  power,              // 6
  occupancy,					// 7
  reed,								// 8 kontaktron
  sig2,               // 9  //in case of relay
  sig3,               // 10  //in case of relay
  switch2,            // 11 
  switch3,            // 12
  xswitch,            // 13
  
};

enum signalType {
  sem4=0,           // 0 czerwony/ziel/zó³ty/bialy
  sem3	  ,         // 1  czerwony/ziel/zó³ty
  lin3,             // 2  liniowy // stan automatyczny
  man2,             // 3  nieb/bialy
};

enum signalVal {
  red=0,           // 0 czerwony/ziel/zó³ty/bialy
  yellow	  ,         // 1  czerwony/ziel/zó³ty
  green,             // 2  liniowy // stan automatyczny
  white,             // 3  nieb/bialy
};

enum blocktype {
	bl_signal=0,
	bl_switch,
	bl_sensor,
	bl_block,
	bl_end,
};

/*
enum dccAnalogType {
  proximity_a=0,				//0 analog only zblizeniowy
  no_dcc_a  ,         //1

};
*/

// -------------------------------------- Arduino ------------------------------

struct Arduino {
  char name[NAME_MAX] ; // board name 
  unsigned short board : 4; //board number will be use for emergency stop in the future
  unsigned short i2c : 7; //i2c bus number

  unsigned short digital : DIGITAL_MAX; 	//No of used digital ports
  unsigned short analog  : ANALOG_MAX;  	//No of used analog  ports  
  unsigned short led     : LED_MAX;  		//No of used led  ports  
  unsigned short bytes  : 5;  					  //No of used all  ports  

  unsigned long ts; // kiedy ostatnio widziany
  
};

union ArduinoU {
  Arduino a;
  byte b[sizeof(Arduino)];
};


// -------------------------------------- BlockMsg ------------------------------

#define BLOCKMSG_NR_BITS 7
struct BlockMsg {
    unsigned short block    : 1;			// block 1/ unblock 0
    unsigned short typ      : 3;    // blocktype
    unsigned short nr       : BLOCKMSG_NR_BITS;			// number
};

union BlockMsgU {
  BlockMsg a;
  byte b[sizeof(BlockMsg)];
};

// -------------------------------------- LedPin ------------------------------

struct LedPin {
    char name[NAME_MAX] ; // pin name 
    unsigned short nr       : NRSIG_LEN;    
    unsigned short port     : DIGITAL_MAX; // 0-15,16-31 // first port of led
    unsigned short dcc       : 10;
    unsigned short signal_type: SIGNAL_TYPE_LEN;
};

union LedPinU {
  LedPin a;
  byte b[sizeof(LedPin)];
};


// -------------------------------------- DigitalPin ------------------------------

struct DigitalPin {
    char name[NAME_MAX] ; // pin name 
    unsigned short nr       : NRSEN_LEN;    
    unsigned short spi       : 3;    //1-7 SPI number 0- arduino
    unsigned short port     : DIGITAL_MAX; // arduino digital port  0-19 32-47 Iexpander 48-63 IIexpander or 0-51 Mega
    unsigned short pull_up  : 1;    
    unsigned short in_port   : 1;    
    unsigned short dcc       : 10;
    unsigned short dcc_type  : 4;
};

union DigitalPinU {
  DigitalPin a;
  byte b[sizeof(DigitalPin)];
};

// -------------------------------------- AnalogPin ------------------------------

struct AnalogPin {
    char name[NAME_MAX] ; // pin name 
    unsigned short nr       : NRSEN_LEN;    
    unsigned short port:ANALOG_MAX; // port number
    unsigned int threshold:10;    // analog port treshold --> below LOW(0) :-) over HIGH(1)
    unsigned short dcc_type  : 2;
};

union AnalogPinU {
  AnalogPin a;
  byte b[sizeof(AnalogPin)];
};



#endif
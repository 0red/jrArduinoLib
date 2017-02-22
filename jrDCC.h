#ifndef JRDCC_h
#define JRDCC_h

// if You use more than A7 needs to chage to 4
// 4 max 16 Analog ports (Mega) 3 max A7 (Nano)
#define ANALOG_MAX 		4
// 6 max 64 Digital ports (Mega) 3 max A7 (Nano)
#define DIGITAL_MAX 	6
// max allowed module and track length
#define NAME_MAX 			10

#define SIZE_LOCAL ANALOG_LOCAL+DIGITAL_LOCAL+1
// +1 for bit informing for pending message to MASTER

#define PROTOCOL_VER 	1


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
  keep_alive,
  //send Arduino board info and reset quere
  hand_shake,
  //reset Pin quere and send first Digital
  reset_pin_info,
  // send NEXT Digial Pin and after Analog Pin or NULL if no more Pin Info
  get_pin_info, 
  // send NEXT Digial Pin info or NULL if no more Digital Pin Info
  // get all states bits (0 no trigger, 1 triggers occupied on) based on the Digital+Analog info order
  get_states,
  // return 2bytes Analog value of the PIN + 2bytes treshhold(4bytes)
  get_Apin,
  // set the Pin state -->0,1
  setD,
  // set the threshold
  set_Atreshold,
  // receive message from I2C Master
  msg_pending,
  
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
  no_dcc=0  ,         //0
  proximity,					// (analog only) zblizeniowy
  light,              //
  power,              // 	
  occupancy,					//
  reed,								// kontaktron
  sig2,               //
  switch2,            //
  sig3,               //
  switch3,            //
  xswitch,            //
};

enum dccAnalogType {
  proximity_a=0,				//0 analog only zblizeniowy
  no_dcc_a  ,         //1

};

// -------------------------------------- Arduino ------------------------------

struct Arduino {
  char name[NAME_MAX] ; // board name 
  unsigned short board : 4; //board number
  unsigned short i2c : 7; //i2c bus number

  unsigned short digital : DIGITAL_MAX; //No of used digital ports
  unsigned short analog  : ANALOG_MAX;  //No of used analog  ports  
  unsigned short bytes  : 5;  //No of used analog  ports  

  unsigned long ts; // kiedy ostatnio widziany
  
};

union ArduinoU {
  Arduino a;
  byte b[sizeof(Arduino)];
};


// -------------------------------------- DigitalPin ------------------------------

struct DigitalPin {
    char name[NAME_MAX] ; // pin name 
    unsigned short spi       : 2;    //1-3 SPI number 0- arduino
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
    unsigned short port:ANALOG_MAX; // port number
    unsigned int threshold:10;    // analog port treshold --> below LOW(0) :-) over HIGH(1)
    unsigned short dcc_type  : 2;
};

union AnalogPinU {
  AnalogPin a;
  byte b[sizeof(AnalogPin)];
};



#endif
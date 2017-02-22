#define DEBUG
#include "Arduino.h"
#include "jr_mcp23s08.h"
#include "jr.h"


// Define SPI-related pins



JRmcp23s08::JRmcp23s08(){
	_haen_prepare(PIN_SLAVE_SELECT,0,0);
	}
	
JRmcp23s08::JRmcp23s08(int cs) { // no HAEN
	_haen_prepare(cs,0,0);
	_haen=0;
	
}

JRmcp23s08::JRmcp23s08(int cs,int a0,int a1) {
	_haen_prepare(cs,a0,a1);
}

void JRmcp23s08::_haen_prepare(int cs,int a0,int a1) {

	 // HAEN + a0,a1 set
	_cs=cs;
	_haen=1;
	a0=(a0)?1:0;
	a1=(a1)?1:0;
	
	a1= (a1 << 2);  // TODO: Allow non-zero and define constants 
	a0= (a0 << 1);  // TODO: Allow non-zero and define constants 
	//(SLAVE_ADDRESS_BASE|SLAVE_ADDRESS_BIT_A1|SLAVE_ADDRESS_BIT_A0)

        
	//  byte deviceOpcode = 0;
    
  _deviceOpcodeRead  = (SLAVE_ADDRESS_BASE | byte(a1) | byte(a0) | CONTROL_BIT_READ);
  _deviceOpcodeWrite = (SLAVE_ADDRESS_BASE | byte(a1) | byte(a0) | CONTROL_BIT_WRITE);

 }



char JRmcp23s08::_spi_transfer(volatile char data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait for the end of the transmission
  {
  };
  return SPDR;                    // return the received byte
}


byte JRmcp23s08::getR(byte registerAddress) {
	return _getRegister(_deviceOpcodeRead,registerAddress);
};


void JRmcp23s08::setR(byte registerAddress, byte value) {
	_setRegister(_deviceOpcodeWrite, registerAddress, value);
};

byte JRmcp23s08::getPins() {
	return getR(9);
}
	
void JRmcp23s08::setPins(byte value) {
	setR(9,value);
}


byte JRmcp23s08::_getRegister(byte targetDeviceOpcode, byte registerAddress) {

	//DEBUG_PRINT("");JR_VD(_cs);JR_PRINT("LOW"); JR_VD(targetDeviceOpcode);JR_VD(registerAddress);
   digitalWrite(_cs, LOW); // Enable slave
  _spi_transfer(targetDeviceOpcode);
  _spi_transfer(registerAddress);  // The register we want to read
  
  byte data; // Correct type?
  data = _spi_transfer(0xFF); // Transfer dummy byte to get response
  digitalWrite(_cs, HIGH); // Disable slave
  //JR_VB(data);JR_PRINT("HIGH");JR_LN;
 
  return data;
} 


void JRmcp23s08::_set_bit(byte reg,byte Apin,int high) {
	if (Apin<8) {
		byte b[]={getR(reg)};
		if (high) {
			SetBit(b,Apin);
		} else {
			ClearBit(b,Apin);
		}
		setR(reg,b[0]);
	}
}

boolean JRmcp23s08::_get_bit(byte reg,byte Apin) {
	if (Apin<8) {
		byte b[]={getR(reg)};
		return (GetBit(b,Apin)>0?true:false);
	}
	return false;
}


void JRmcp23s08::pinWrite(byte Apin,int high) {
	_set_bit(9,Apin,high);
}

boolean JRmcp23s08::pinRead(byte Apin) {
	return _get_bit(9,Apin);
}


void JRmcp23s08::setIN(byte Apin, boolean _Ain) {
	_set_bit(0,Apin,_Ain);
}

boolean JRmcp23s08::getIN(byte Apin) {
	return _get_bit(0,Apin);
}

void JRmcp23s08::setPullUp(byte Apin, boolean _Ain) {
	_set_bit(6,Apin,_Ain);
}

boolean JRmcp23s08::getPullUp(byte Apin){
	return _get_bit(6,Apin);
}



void JRmcp23s08::_setRegister(byte targetDeviceOpcode, byte registerAddress, byte value) {
  // TODO: Do Better?

	//DEBUG_PRINT("");JR_VD(_cs);JR_PRINT("LOW"); JR_VD(targetDeviceOpcode);JR_VD(registerAddress);JR_VB(value);
  digitalWrite(_cs, LOW); // Enable slave
  _spi_transfer(targetDeviceOpcode);
  _spi_transfer(registerAddress);  // The register we want to write
  _spi_transfer(value);
  
  digitalWrite(_cs, HIGH); // Disable slave
	//JR_PRINT("HIGH");JR_LN;
} 


  
 void JRmcp23s08::initialize() {

  pinMode(PIN_DATA_OUT, OUTPUT);
  pinMode(PIN_DATA_IN, INPUT);
  pinMode(PIN_SPI_CLOCK, OUTPUT);
  pinMode(_cs, OUTPUT);
  
  digitalWrite(_cs, HIGH); // Disable slave

  //    Configure SPI Control Register (SPCR) (All values initially 0)
  //     Bit  Description
  //       7  SPI Interrupt Enable    -- disable  (SPIE --> 0)
  //       6  SPI Enable              -- enable   (SPE  --> 1)
  //       5  Data Order              -- MSB 1st  (DORD --> 0) (Slave specific)
  //       4  Master/Slave Select     -- master   (MSTR --> 1)
  //       3  Clock Polarity          --          (CPOL --> 0) (Slave specific) ("Mode")
  //       2  Clock Phase             --          (CPHA --> 0) (Slave specific)
  //       1  SPI Clock Rate Select 1 -- }        (SPR1 --> 0) 
  //       0  SPI Clock Rate Select 0 -- } fOSC/4 (SPR0 --> 0) ("Fastest" but see SPI2X in SPSR)
  SPCR = (1<<SPE)| (1<<MSTR);
  // Clear previous data and status (TODO: Determine if necessary/better way.)
  // (Based on Playground SPI example.)
  byte dummy;
  dummy = SPSR;
  dummy = SPDR;
  delay(10);
    
  // Serial.println((1 << SPIF), BIN);

	for (int i=0;i<11;i++) _setRegister(_deviceOpcodeWrite,i,0);

  byte r=_getRegister(_deviceOpcodeRead,5);
  r |= SEQOP_DISABLED;
  
  if (_haen) {
		_setRegister (_deviceOpcodeWrite,5,r | (1<<3));
		JR_PRINTBINV("haen set",(r | (1<<3)));JR_LN;
  } else {
		_setRegister (_deviceOpcodeWrite,5,r & (0<<3));
		JR_PRINTBINV("haen clear",(r & (0<<3)));JR_LN;
  }
  
  JR_VD(_cs);JR_LN;
  JR_VB(_haen);JR_LN;  
  JR_VB(_deviceOpcodeRead);JR_VD(_deviceOpcodeRead);JR_LN;
  JR_VB(_deviceOpcodeWrite);JR_VD(_deviceOpcodeWrite);JR_LN;
  
  
  
}

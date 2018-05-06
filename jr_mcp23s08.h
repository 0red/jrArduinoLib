#ifdef __AVR__
#ifndef JR_mcp23s08_h
#define JR_mcp23s08_h

// Define SPI-related pins
#define PIN_DATA_OUT 11 // MOSI (Master Out / Slave In)
#define PIN_DATA_IN  12 // MISO (Master In / Slave Out)
#define PIN_SPI_CLOCK  13 // SCK (Serial Clock) 
#define PIN_SLAVE_SELECT 10 // SS (Slave Select)

#include "Arduino.h"

//#include <inttypes.h>

# define SLAVE_ADDRESS_BASE    (B01000 << 3)
# define CONTROL_BIT_READ 1
# define CONTROL_BIT_WRITE 0

// 0 enable non 0 disable
# define SEQOP_DISABLED B00100000
// # define SEQOP_DISABLED 0 

# define REG_IODIR 0x00
# define REG_IOPOL 0x01

# define REG_GPIO 0x09



class JRmcp23s08 {
 public:
	JRmcp23s08(); //SC=10 no HAEN // a0&a1 must be grounded (0)
	JRmcp23s08(int cs); // no HAEN // a0&a1 must be grounded (0)
	JRmcp23s08(int cs,int a0,int a1); // HAEN + a0,a1 set
	
	byte getR(byte registerAddress);
	void setR(byte registerAddress, byte value);

	void   pinWrite(byte Apin,int high);
	boolean pinRead(byte Apin);
	
	byte 		getPins();
	void 		setPins(byte value);
	
	void    setIN(byte Apin, boolean _Ain);
	boolean getIN(byte Apin);

	void    setPullUp(byte Apin, boolean _Ain);
	boolean getPullUp(byte Apin);

	void initialize();  // to initialize the mcp23s08 (put 0 to all registry then assign _haen if needed
	 

 protected:
	byte _deviceOpcodeRead = 0;
	byte _deviceOpcodeWrite = 0; // TODO: handle this better?
	byte _cs=0;
	byte _haen=0;
	
	char _spi_transfer(volatile char data);
	byte _getRegister(byte targetDeviceOpcode, byte registerAddress);
	void _setRegister(byte targetDeviceOpcode, byte registerAddress, byte value);
	void _haen_prepare(int cs,int a0,int a1); // 
	void _set_bit(byte reg,byte Apin,int high) ;
	boolean _get_bit(byte reg,byte Apin) ;


};

#endif
#endif
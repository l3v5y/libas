/*
  libas.h - Arduino library for Austria Microsystems AS5145 and AS5045
  Copyright (c) 2011 Oliver Levett.  All right reserved.
*/

// ensure this library description is only included once
#ifndef LIBAS_H
#define LIBAS_H
 
// include types & constants of Wiring core API
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WConstants.h"
#endif


union ASDataFlags
{
	byte data;
	struct
	{
		byte OCF : 1; 
		byte COF : 1;
		byte LIN : 1;
		byte MagInc : 1;
		byte MagDec:1;
		byte EvenPar:1;
	};
};

class libas
{
	public:
		// Default constructor, SCLK connects to CLK, CSn to ChipSelect and DI to DO on AS5X45
		libas(int CLK, int DI, int CSn);
		// Read position off sensor, and update
		int ASGetPosition(void);
		// Read position off sensor and return flags (wraps ASGetPosition for code size)	
		ASDataFlags ASGetDataFlags(void);
		// Flags from last read of data
		ASDataFlags Flags;
		// Position, for 10bit 0-1023, 12bit 0-4095
		int Position;
	private:
		// Precision (0 based, 9 is 10bit, 11 is 12bit)
		int DataPrecision;
		int chipSelectPin;
		int clkPin;
		int dInPin;
};

// LIBAS_H
#endif 

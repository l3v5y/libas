/*
  libas.cpp - Arduino library for Austria Microsystems AS5145 and AS5045
  Copyright (c) 2011 Oliver Levett.  All right reserved.
*/

// include core Wiring API
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// include this library's description file
#include "libas.h"

// Default constructor, SCLK connects to CLK, CSn to ChipSelect and DI to DO on AS5X45
libas::libas(int CLK, int DI, int CS)
{
	// Set pins up for output/input
	clkPin = CLK;
	dInPin = DI;
	chipSelectPin = CS;
	pinMode(CLK, OUTPUT);
	pinMode(DI, INPUT);
	pinMode(CS, OUTPUT);
	// DataPrecision is 12bit
	DataPrecision = 12;
}

// Read position off sensor and return flags (wraps ASGetPosition for code size)
ASDataFlags libas::ASGetDataFlags(void)
{
	ASGetPosition();
	return Flags;
}
// Read position off sensor, and update
int libas::ASGetPosition(void)
{
	// Temp position
	int tempPosition = 0;
	// Counter
	int i = 0;
	// XOR for parity check (Even Parity)
	byte XOR = 0;
	// Temp reading byte
	byte tempRead = 0;
	// Select sensor
	digitalWrite(chipSelectPin, LOW);  	
	
	// Sensor feeds out position MSB first
	for(i = DataPrecision-1; i >= 0; i--)
	{
		// Toggle clock line, wait, and then read the data
		digitalWrite(clkPin,LOW);
		// Delays must be at least 500ns
		delayMicroseconds(1);
		digitalWrite(clkPin,HIGH);    
		delayMicroseconds(1);
		tempRead = digitalRead(dInPin)&0x01;
		// XOR data for checksum
		XOR ^= tempRead;
		// Read to tempPosition so we can buffer if data is invalid
		tempPosition |= (tempRead)<<i;
		
	}

	// Status bits are fed out
	for(i = 0; i < 6; i++)
	{
		// Toggle clock line, wait, and then read the data
		digitalWrite(clkPin,LOW);
		delayMicroseconds(1);
		digitalWrite(clkPin,HIGH);    
		delayMicroseconds(1);
		tempRead = digitalRead(dInPin)&0x01;
		// XOR data for checksum
		XOR ^= tempRead;	
		// We don't buffer Flags, so user can check for errors
		Flags.data |= (tempRead)<<i;
	}
	
	// Disable chipselect
	digitalWrite(chipSelectPin, HIGH);

	// ValidityCheck
	if(XOR == 0
		&& Flags.OCF == 1
		&& Flags.COF == 0
		&& Flags.LIN == 0)
	{
		// if valid, return flags and update position
		Position=tempPosition;
	}
	return Position;
}

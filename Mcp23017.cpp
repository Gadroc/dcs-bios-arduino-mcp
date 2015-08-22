#include "Mcp23017.h"
#include <Wire.h>

#define IODIRA    (byte)0x00
#define IODIRB    (byte)0x01
#define IPOLA    (byte)0x02
#define IPOLB    (byte)0x03
#define GPINTENA    (byte)0x04
#define GPINTENB    (byte)0x05
#define DEFVALA    (byte)0x06
#define DEFVALB   (byte)0x07
#define INTCONA    (byte)0x08
#define INTCONB   (byte)0x09
#define IOCONA      (byte)0x0A
#define IOCONB      (byte)0x0B
#define GPPUA      (byte)0x0C
#define GPPUB      (byte)0x0D
#define INTFA      (byte)0x0E
#define INTFB      (byte)0x0F
#define INTCAPA   (byte)0x10
#define INTCAPB   (byte)0x11
#define GPIOA      (byte)0x12
#define GPIOB      (byte)0x13
#define OLATA      (byte)0x14
#define OLATB      (byte)0x15

Mcp23017::Mcp23017(byte address, uint8_t pollInterval)
{
	_nextPoll = millis();
	_address = address;
	_pollInterval = pollInterval;
    for(int i=0;i<16;i++) {
        _inputPins[i].init(this, i);
    }	
}

void Mcp23017::begin()
{
	Wire.beginTransmission(_address);
	Wire.write(IPOLA);
	Wire.write((uint8_t)0xFF);
	Wire.write((uint8_t)0xFF);
	Wire.endTransmission();

	Wire.beginTransmission(_address);
	Wire.write(GPPUA);
	Wire.write((uint8_t)0xFF);
	Wire.write((uint8_t)0xFF);
	Wire.endTransmission();
	
	poll();
}

void Mcp23017::poll()
{
	long current = millis();
	if (current - _nextPoll > 0)
	{
		Wire.beginTransmission(_address);
		Wire.write((uint8_t)GPIOA);
		Wire.endTransmission();
		
		Wire.requestFrom(_address,(uint8_t)2);
		while (Wire.available() < 2) {}

		_bank1 = Wire.read();
		_bank2 = Wire.read();
		_nextPoll = current + _pollInterval;
	}
}
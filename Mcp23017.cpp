/*
	Copyright 2015 Craig Courtney

    This file is part of DcsBios-Firmware.

    DcsBios-Firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DcsBios-Firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DcsBios-Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Mcp23017.h"
#include <Wire.h>
#include "Mcp.h"

Mcp23017::Mcp23017(byte address, uint8_t pollInterval)
{
	_nextPoll = millis();
	_address = address;
	_pollInterval = pollInterval;
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
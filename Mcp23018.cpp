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
#include "Mcp23018.h"
#include <Wire.h>
#include "Mcp.h"

Mcp23018::Mcp23018(uint8_t address)
{
    _portAState = 0xFF;
    _portBState = 0xFF;
    _address = address;
}

void Mcp23018::begin() {
    Wire.beginTransmission(_address);
    Wire.write(IODIRA);
    Wire.write((uint8_t)0x00);
    Wire.write((uint8_t)0x00);
    Wire.write((uint8_t)0xFF);
    Wire.write((uint8_t)0xFF);
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(GPIOA);
    Wire.write((uint8_t)0xFF);
    Wire.write((uint8_t)0xFF);
    Wire.endTransmission(); 
}

void Mcp23018::setState(uint8_t pin, bool state)
{
    if (pin < 8)
    {
        if (state)
        {
            _portAState &= ~(1 << pin);
        }
        else
        {
            _portAState |= 1 << pin;
        }           
    }
    else if (pin < 16)
    {
        if (state)
        {
            _portBState &= ~(1 << (pin-8));
        }
        else
        {
            _portBState |= 1 << (pin-8);
        }           
    }
    else 
    {
        // This exandper only has 16 outputs.
        return;
    }
}

void Mcp23018::onBufferReady(uint8_t *buffer) {
    Wire.beginTransmission(_address);
    Wire.write(GPIOA);
    Wire.write((uint8_t)_portAState);
    Wire.write((uint8_t)_portBState);
    Wire.endTransmission();
}
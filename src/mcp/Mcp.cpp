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
#include "Mcp.h"
#include <Wire.h>

Mcp::Mcp(uint8_t address, uint8_t portAOutputMask, uint8_t portBOutputMask, uint8_t pollInterval)
{
    _nextPoll = millis()-1;
    _address = address;
    _portAOutputMask = portAOutputMask;
    _portBOutputMask = portBOutputMask;
    _pollInterval = pollInterval;
}

void Mcp::begin()
{

    Wire.beginTransmission(_address);
    Wire.write(IODIRA);
    Wire.write((uint8_t)~_portAOutputMask);
    Wire.write((uint8_t)~_portBOutputMask);
    Wire.write((uint8_t)0xFF);
    Wire.write((uint8_t)0xFF);
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(GPIOA);
    Wire.write((uint8_t)_portAOutputMask);
    Wire.write((uint8_t)_portBOutputMask);
    Wire.endTransmission();

    Wire.beginTransmission(_address);
    Wire.write(GPPUA);
    Wire.write((uint8_t)~_portAOutputMask);
    Wire.write((uint8_t)~_portBOutputMask);
    Wire.endTransmission();

    poll();
}

void Mcp::poll()
{
    long current = millis();
    if (current - _nextPoll > 0)
    {
        Wire.beginTransmission(_address);
        Wire.write((uint8_t)GPIOA);
        Wire.endTransmission();

        Wire.requestFrom(_address,(uint8_t)2);
        while (Wire.available() < 2) {}

        _portAInputState = Wire.read();
        _portBInputState = Wire.read();
        _nextPoll = current + _pollInterval;
    }
}

uint8_t Mcp::readPinState(uint8_t pin) {
    poll();

    if (pin < 8) {
        return _portAInputState & (1 << pin) ? LOW : HIGH;
    } else if (pin < 16) {
        return _portBInputState & (1 << 8-pin) ? LOW : HIGH;
    } else {
        // This exandper only has 16 outputs.
        return LOW;
    }
}

void Mcp::setPinState(uint8_t pin, bool state)
{
    if (pin < 8) {
        if (state) {
            _portAOutputState &= ~(1 << pin);
        } else {
            _portAOutputState |= 1 << pin;
        }
    } else if (pin < 16) {
        if (state) {
            _portBOutputState &= ~(1 << (pin-8));
        } else {
            _portBOutputState |= 1 << (pin-8);
        }
    } else {
        // This exandper only has 16 outputs.
        return;
    }
}


void Mcp::onDcsBiosFrameSync() {
    Wire.beginTransmission(_address);
    Wire.write(GPIOA);
    Wire.write((uint8_t)(_portAOutputState & _portAOutputMask));
    Wire.write((uint8_t)(_portBOutputState & _portAOutputMask));
    Wire.endTransmission();
}

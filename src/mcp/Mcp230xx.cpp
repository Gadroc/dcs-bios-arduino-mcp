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
#include "Mcp230xx.h"
#include <Wire.h>

uint8_t Mcp230xxPort::readPinState(uint8_t pin) {

    // If invalid pin number return LOW 
    if (pin >= 8) {
        return LOW;
    }

    // Read polarity is inverted in hardware if necessary
    // through IPOLA/IPOLB register in begin().
    uint8_t pinMask = 1 << pin;
    return state & pinMask ? HIGH : LOW;
}

void Mcp230xxPort::setPinState(uint8_t pin, bool pinState) {
    uint8_t pinMask = 1 << pin;

    // Return early if invalid pin number or pin is
    // setup for input.
    if (pin >= 8 | (pinDirection & pinMask) > 0) {
        return;
    }

    if (pinPolarityInvert & pinMask) {
        pinState = !pinState;
    }

    if (pinState) {
        state |= pinMask;
    } else {
        state &= ~pinMask;
    }

    changed = true;
}

Mcp230xx::Mcp230xx(uint8_t address, uint8_t pollInterval) : _address(address)
{
    // Trigger pollin on next read
    _nextPoll = millis()-1;
    _pollInterval = pollInterval;
}

void Mcp230xx::begin(uint16_t ioDir, uint16_t pullUp, uint16_t polarityInvert)
{
    _portA.state = 0x00;
    _portA.pinDirection = ioDir & 0xff;
    _portA.pinPolarityInvert = polarityInvert & 0xff;
    _portA.changed = false;

    _portB.state = 0x00;
    _portB.pinDirection = (ioDir >> 8) & 0xff;
    _portB.pinPolarityInvert = (polarityInvert >> 8) & 0xff;
    _portB.changed = false;

    // Set pins to input/output mode
    Wire.beginTransmission(_address);
    Wire.write(IODIRA);
    Wire.write((uint8_t)_portA.pinDirection); // IODIRA
    Wire.write((uint8_t)_portB.pinDirection); // IODIRB
    Wire.write((uint8_t)_portA.pinPolarityInvert); // IPOLA
    Wire.write((uint8_t)_portB.pinPolarityInvert); // IPOLB
    Wire.endTransmission();

    // Turn on internal pull ups for input pins
    Wire.beginTransmission(_address);
    Wire.write(GPPUA);
    Wire.write((uint8_t)(pullUp & 0xff)); // GPPUA
    Wire.write((uint8_t)((pullUp >> 8) & 0xff)); // GPPUB    
    Wire.endTransmission();

    for (int i=0; i<16; i++) {
        setPinState(i, false);
    }

    onDcsBiosFrameSync();

    poll();
}

void Mcp230xx::poll()
{
    long current = millis();
    if (current - _nextPoll > 0)
    {
        Wire.beginTransmission(_address);
        Wire.write((uint8_t)GPIOA);
        Wire.endTransmission();

        Wire.requestFrom(_address,(uint8_t)2);
        while (Wire.available() < 2) {}

        _portA.state = Wire.read();
        _portB.state = Wire.read();

        _nextPoll = current + _pollInterval;
    }
}

uint8_t Mcp230xx::readPinState(uint8_t pin) {
    poll();

    if (pin < 8) {
        return _portA.readPinState(pin);
    } else if (pin <16) {
        return _portB.readPinState(pin-8);
    }
}

void Mcp230xx::setPinState(uint8_t pin, bool pinState)
{
    if (pin < 8) {
        _portA.setPinState(pin, pinState);
    } else {
        _portB.setPinState(pin-8, pinState);
    }
}


void Mcp230xx::onDcsBiosFrameSync() {
    if (_portA.changed || _portB.changed) {
        Wire.beginTransmission(_address);
        Wire.write(GPIOA);
        Wire.write((uint8_t)(_portA.state & ~_portA.pinDirection));
        Wire.write((uint8_t)(_portB.state & ~_portB.pinDirection));
        Wire.endTransmission(); 
        _portA.changed = false;
        _portB.changed = false;   
    }
}

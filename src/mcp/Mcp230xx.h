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
#ifndef _DCSBIOS_MCP230xx_H_
#define _DCSBIOS_MCP230xx_H_

#include <Arduino.h>
#include "Mcp.h"
#include "dcs/ExportStreamListener.h"

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

class Mcp230xxPort {
public:
    uint8_t state;
    uint8_t pinDirection;
    uint8_t pinPolarityInvert;
    bool changed;

    uint8_t readPinState(uint8_t pin);
    void setPinState(uint8_t pin, bool pinState);
};


// Class that exposes the pins on a MCP23017/MCP23018 chip
// to DCS bios as input pins.
class Mcp230xx : public Mcp {
private:
    const uint8_t _address;

    long _nextPoll;
    uint8_t _pollInterval;

    bool _outputChanged;

    Mcp230xxPort _portA;
    Mcp230xxPort _portB;

    void poll();
    void onDcsBiosFrameSync() override;

public:
    explicit Mcp230xx(uint8_t address, uint8_t pollInterval = 10);

    void begin(uint16_t ioDir, uint16_t pullUps = 0x0000, uint16_t polarityInvert = 0x0000);
    void setPinState(uint8_t pin, bool state) override;
    uint8_t readPinState(uint8_t pin) override;
};

#endif
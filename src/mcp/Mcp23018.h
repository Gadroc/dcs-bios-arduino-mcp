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
#ifndef _DCSBIOS_MCP23018_H_
#define _DCSBIOS_MCP23018_H_

#include <Arduino.h>
#include "dcs/ExportStreamListener.h"
#include "hal/OutputPin.h"

class Mcp23018 : ExportStreamListener {
private:
    class McpOutputPin : public OutputPin {
    private:
        Mcp23018& _mcp;
        uint8_t _pin;

    public:
        McpOutputPin(Mcp23018& mcp, uint8_t pin) : _mcp(mcp) {
            _pin = pin;
        }

        virtual void set() {
            _mcp.setState(_pin, true);
        }

        virtual void clear() {
            _mcp.setState(_pin, false);
        }
    };

    uint8_t _address;
    uint8_t _portAState;
    uint8_t _portBState;

    // Sets the power state for an LED on this bank.
    // led - led to set the power for (0-15)
    // power - state of the power (true = on, false = off)
    void setState(uint8_t pin, bool state);

    virtual void onDcsBiosFrameSync();

public:
    Mcp23018(uint8_t address);

    // Initializes the LED bank.  Should be called from setup().
    void begin();
    OutputPin& getPin(uint8_t pin) {
        return *(new McpOutputPin(*this, pin));
    }
};

#endif
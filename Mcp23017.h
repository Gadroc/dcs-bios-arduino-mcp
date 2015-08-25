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
#ifndef _DCSBIOS_MCP23017_H_
#define _DCSBIOS_MCP23017_H_

#include <Arduino.h>
#include "InputPin.h"

class Mcp23017 {
private:
    class McpInputPin : public InputPin {
    private:
        Mcp23017* _mcp;
        uint8_t _pin;
        uint8_t _mask;

    public:
        McpInputPin(Mcp23017* mcp, uint8_t pin) {
            _mcp = mcp;
            _pin = pin;
            if (pin > 7) { pin -= 8; }
            _mask = 1<<pin;
        }

        virtual uint8_t readState() {
            _mcp->poll();
            if (_pin > 7) {
                return _mcp->_bank2 & _mask ? LOW : HIGH;
            } else {
                return _mcp->_bank1 & _mask ? LOW : HIGH;
            }
        }
    };

    long _nextPoll;
    uint8_t _pollInterval;
    uint8_t _address;
    void poll();
    uint8_t _bank1;
    uint8_t _bank2;

public:
    Mcp23017(uint8_t address, uint8_t pollInterval = 10);

    void begin();

    InputPin* getPin(uint8_t pin) {
        return new McpInputPin(this, pin);
    }
};

#endif
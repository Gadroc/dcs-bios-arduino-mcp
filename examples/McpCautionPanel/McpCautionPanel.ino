#include <Arduino.h>
#include <Wire.h>
#include <DcsBios.h>
#include <DcsBiosMcp.h>

/*
 * This is a partial example of implementing a caution panel with an
 * MCP23018.  Be careful as an MCP23017 can not sink nearly as much 
 * current.  Check your datasheets before implementing.
 */

Mcp230xx expander(B0100000);

/* Instantiate a device object to parse the DCS-BIOS export stream */
DcsBiosSerialDevice dcsBiosDevice(Serial);

McpPin clA1pin(expander, 0);
Led clA1(0x10d4, 0x0001, 0, clA1pin);

McpPin clA2pin(expander, 1);
Led clA2(0x10d4, 0x0002, 1, clA2pin);

McpPin clA3pin(expander, 2);
Led clA3(0x10d4, 0x0004, 2, clA3pin);

McpPin clA4pin(expander, 3);
Led clA4(0x10d4, 0x0008, 3, clA4pin);

McpPin clA5pin(expander, 4);
Led clA5(0x10d4, 0x0010, 4, clA5pin);

McpPin clA6pin(expander, 5);
Led clA6(0x10d4, 0x0020, 5, clA6pin);

McpPin clA7pin(expander, 6);
Led clA7(0x10d4, 0x0040, 6, clA7pin);

McpPin clA8pin(expander, 7);
Led clA8(0x10d4, 0x0080, 7, clA8pin);

/*
 * Your setup routine needs to intialize your communications
 * and all of your input devices.
 */
void setup() {
    // Initialize the serial port to 250000 baud.
    Serial.begin(250000);

    // Initialize I2C bus
    Wire.begin();
    // Set to highspeed clock
    Wire.setClock(400000L);

    // Initialize MCP Expander
    // Since the LEDs are connected to an MCP on the ground side we invert
    // the outputs.  This allows the MCP23018 to sink the current instead of
    // supplying it.  This allows us to drive more current through the MCP23018.
    expander.begin(0b0000000000000000, 0b0000000000000000, 0b1111111111111111);
    
    // Initialize all of your polling inputs.
    PollingInput::initInputs();
}

/**
 * Your main loop needs to process bus data and poll your inputs.
 */
void loop() {
    // Process any incoming data.
    dcsBiosDevice.process();

    // Check our inputs and record messages for DCS-BIOS
    PollingInput::pollInputs();
}

/*
 * This method will get called at the end of every data frame.  You
 * can update any outputs here if necessary.  Built in output devices
 * automatcially update, and don't need code here.
 */
void onDcsBiosFrameSync() {
}

/*
 * You need to define
 * void sendDcsBiosMessage(const char* msg, const char* arg)
 * so that the string msg, followed by a space, the string arg
 * and a newline gets sent to the DCS-BIOS import stream.
 * Typically we will use the built in function of the serial
 * DcsBiosSerialDevice or DcsBiosRs485Device to send data back.
 * But you can use this method to manually use other communication
 * techniques.
 */
void sendDcsBiosMessage(const char* msg, const char* arg) {
    dcsBiosDevice.sendDcsBiosMessage(msg, arg);
}
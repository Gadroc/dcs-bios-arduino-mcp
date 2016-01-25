#include <Arduino.h>
#include <Wire.h>
#include <DcsBios.h>
#include <DcsBiosMcp.h>

/* MCP IO Expander
 * Address B0100000 - Default address if all address lines are tied to ground on MCP23017/MCP23018
 */
Mcp230xx expander(B0100000);

/* Instantiate a device object to parse the DCS-BIOS export stream */
DcsBiosSerialDevice dcsBiosDevice(Serial);

/* Setup the CMSP Mode switch on pins 0, 1, 2, 3, 4 (Port A bits 0-4) */
InputPin** cmspModePins = new InputPin*[5] {
    new McpPin(expander, 0),
    new McpPin(expander, 1),
    new McpPin(expander, 2),
    new McpPin(expander, 3),
    new McpPin(expander, 4)
};
SwitchMultiPos cmspMode("CMSP_MODE", cmspModePins, 5);

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
    // * Port A - Setup to inputs, pullups on and no inversion
    // * Port B - Setup to outputs, no inversion
    expander.begin(0b0000000011111111, 0b0000000011111111, 0b0000000000000000);
    
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
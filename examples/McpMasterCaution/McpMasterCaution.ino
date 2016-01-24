#include <Arduino.h>
#include <Wire.h>
#include <DcsBios.h>
#include <DcsBiosMcp.h>

// MCP IO Expander on Address 0b0100001 with default polling interval
Mcp230xx expander(0b0100001);

/* Instantiate a device object to parse the DCS-BIOS export stream */
DcsBiosRs485Device dcsBiosDevice(Serial, 8, 1);

/* Declare a Master Caution Reset button on input pin 0 (Port A bit 0) of the Mcp23017 */
McpPin ufcInputPin(expander, 0);
Switch2Pos masterCautionButton("UFC_MASTER_CAUTION", ufcInputPin);

/* Make the LED connected to pin 8 (Port B bit 0) into a Master Caution Light */
McpPin ufcLedPin(expander, 8);
Led masterCautionLed(0x1012, 0x0800, 11, ufcLedPin);

/*
 * Your setup routine needs to intialize your communications
 * and all of your input devices.
 */
void setup() {
    // Initialize the serial port to 250000 baud.
    Serial.begin(250000);

    // Initialize I2C bus
    Wire.begin();
    Wire.setClock(400000L);

    // Initialize MCP Expander
    // * Port A - Setup to inputs, pullups on and no inversion
    // * Port B - Setup to outputs, no inversion
    expander.begin(0b0000000011111111, 0b0000000011111111, 0b0000000000000000);
    
    // Initialize all of your polling inputs.
    PollingInput::initInputs();

    ufcLedPin.set();
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

cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME DcsBiosMcp)
project(${PROJECT_NAME})

set(DcsBios_RECURSE True)
set(DcsBiosMcp_RECURSE True)
link_directories(..)

set(McpMasterCaution_SKETCH examples/McpMasterCaution/McpMasterCaution.ino)
generate_arduino_firmware(
        McpMasterCaution
)

set(McpSwitchMultiPos_SKETCH examples/McpSwitchMultiPos/McpSwitchMultiPos.ino)
generate_arduino_firmware(
        McpSwitchMultiPos
)

set(McpCautionPanel_SKETCH examples/McpCautionPanel/McpCautionPanel.ino)
generate_arduino_firmware(
        McpCautionPanel
)
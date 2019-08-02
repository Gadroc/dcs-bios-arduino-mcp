cmake_minimum_required(VERSION 3.13.1)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/Arduino-Toolchain.cmake)
set(ARDUINO_CMAKE_SKETCHBOOK_PATH ${CMAKE_SOURCE_DIR}/../..)

project(DcsBiosMcpLibrary)

get_board_id(uno_board_id uno)

set(dcsbios_mcp_sources src/DcsBiosMcp.h src/mcp/Mcp230xx.cpp)
add_arduino_library(dcsbios_mcp_uno_lib ${uno_board_id} ${dcsbios_mcp_sources})
add_arduino_core_lib(dcsbios_mcp_uno_lib ${uno_board_id})
find_arduino_library(dcsbios_uno_lib DcsBios ${uno_board_id} 3RD_PARTY)
link_arduino_library(dcsbios_mcp_uno_lib dcsbios_uno_lib ${uno_board_id})

add_subdirectory(examples)
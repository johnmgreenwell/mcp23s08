//--------------------------------------------------------------------------------------------------------------------
// Name        : mcp23s08.h
// Purpose     : MCP23S08 IO-expander driver class
// Description : 
//               This class intended for device driver control of MCP23S08 SPI IO-expander chips.
//
//               Interrupts may be setup by applying values to the appropriate registers to desired specification as 
//               described in the datasheet. Improvements to this class may add interrupt methods.
//
// Language    : C++
// Platform    : Portable
// Framework   : Portable
// Copyright   : MIT License 2024, John Greenwell
// Requires    : External : N/A
//               Custom   : hal.h - Custom implementation-defined Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------------------------
#ifndef _MCP23S08_H
#define _MCP23S08_H

#include "hal.h"

namespace PeripheralIO
{

// Base I2C address
const uint8_t MCP23S08_ADDR    = 0x20;

// Register map
const uint8_t MCP23S08_IODIR   = 0x00;
const uint8_t MCP23S08_IPOL    = 0x01;
const uint8_t MCP23S08_GPINTEN = 0x02;
const uint8_t MCP23S08_DEFVAL  = 0x03;
const uint8_t MCP23S08_INTCON  = 0x04;
const uint8_t MCP23S08_IOCON   = 0x05;
const uint8_t MCP23S08_GPPU    = 0x06;
const uint8_t MCP23S08_INTF    = 0x07;
const uint8_t MCP23S08_INTCAP  = 0x08;
const uint8_t MCP23S08_GPIO    = 0x09;
const uint8_t MCP23S08_OLAT    = 0x0A;

class MCP23S08 
{
    public:

        /**
         * @brief Constructor for MCP23S08 object
         * @param address Hardwired address (requires only the 2 LSB matching device external biasing)
         * @param spi_channel Identifier able to be used by HAL when more than one SPI channel in parent project
        */
        MCP23S08(uint8_t address, uint8_t cs_pin, uint8_t spi_channel=0);

        /**
         * @brief Initialize MCP23S08 device; device will not work correctly without initialization
        */
        void init() const;

        /**
         * @brief Set IO type on a given pin
         * @param pin Pin 0-7 on which mode is set
         * @param mode Pin mode (GPIO_OUTPUT, GPIO_INPUT, GPIO_INPUT_PULLUP)
         * @return False on invalid arguments, true otherwise
        */
        bool pinMode(uint8_t pin, uint8_t mode) const;

        /**
         * @brief Set IO type on entire 8 pins of MCP23S08
         * @param mode Pin mode (GPIO_OUTPUT, GPIO_INPUT, GPIO_INPUT_PULLUP)
        */
        void portMode(uint8_t mode) const;

        /**
         * @brief Write value to individual pin 0-7; pin must already be configured as output
         * @param pin Pin 0-7 on which value is set
         * @param val Value to be written; zero for logic low, non-zero for logic high
         * @return False on invalid arguments, true otherwise
        */
        bool digitalWrite(uint8_t pin, uint8_t val) const;

        /**
         * @brief Read value from individual pin 0-7; pin must already be configured as input
         * @param pin Pin 0-7 from which value is read
         * @return Value read from pin. Zero for logic low, one for logic high
        */
        uint8_t digitalRead(uint8_t pin) const;

        /**
         * @brief Direct write value to entire GPIO port; port must be already configured as output
         * @param val Value to write to GPIO port
        */
        void write(uint8_t val) const;

        /**
         * @brief Direct write of value to specific MCP23S08 internal register; e.g. write(MCP23S08_OLAT, 0xFF)
         * @param reg Register number to which value is to be written; e.g. MCP23S08_INTCON
         * @param val Value to write to register
        */
        void write(uint8_t reg, uint8_t val) const;

        /**
         * @brief Direct read value from entire GPIO port; port must already be configured as input
         * @return Value read from GPIO port
        */
        uint8_t read() const;

        /**
         * @brief Direct read of value from specific register; e.g. read(MCP23S08_OLAT)
         * @param reg Register number to which value is to be written; e.g. MCP23S08_INTCON
        */
        uint8_t read(uint8_t reg) const;
 
private:
    uint8_t   _address;
    HAL::GPIO _cs_pin;
    HAL::SPI  _spi;

    void spiWrite(uint8_t reg, uint8_t byte) const;
    uint8_t spiRead(uint8_t reg) const;
};

}

#endif // _MCP23S08_H

// EOF

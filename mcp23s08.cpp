//--------------------------------------------------------------------------------------------------------------------
// Name        : mcp23s08.cpp
// Purpose     : MCP23S08 IO-expander driver class
// Description : This source file implements header file mcp23S08.h.
// Language    : C++
// Platform    : Portable
// Framework   : Portable
// Copyright   : MIT License 2024, John Greenwell
//--------------------------------------------------------------------------------------------------------------------

#include "mcp23s08.h"

namespace PeripheralIO 
{

MCP23S08::MCP23S08(HAL::SPI& spi_bus, uint8_t cs_pin, uint8_t address)
: _spi(spi_bus)
, _cs_pin(cs_pin)
, _address((MCP23S08_ADDR | (address & 0x03)) << 1)
{
    _cs_pin.pinMode(GPIO_OUTPUT);
    _cs_pin.digitalWrite(true);
}

void MCP23S08::init() const
{
    _cs_pin.pinMode(GPIO_OUTPUT);
    _cs_pin.digitalWrite(true);
}

bool MCP23S08::pinMode(uint8_t pin, uint8_t mode) const 
{
    uint8_t data = 0;

    if (pin > 7) return false;

    data = spiRead(MCP23S08_IODIR);
    
    if (mode == GPIO_INPUT) 
    { 
        data |= (1 << pin);
    } 
    else if (mode == GPIO_OUTPUT) 
    {
        data &= ~(1 << pin);
    } 
    else if (mode == GPIO_INPUT_PULLUP)
    {
        data |= (1 << pin);
        spiWrite(MCP23S08_GPPU, spiRead(MCP23S08_GPPU) | (1 << pin));
    } 
    else 
    {
        return false;
    }

    spiWrite(MCP23S08_IODIR, data);
    
    return true;
}

void MCP23S08::portMode(uint8_t mode) const 
{
    if (mode == GPIO_INPUT) 
    { 
        spiWrite(MCP23S08_GPPU, 0x00);
        spiWrite(MCP23S08_IODIR, 0xFF);
    } 
    else if (mode == GPIO_OUTPUT) 
    {
        spiWrite(MCP23S08_GPPU, 0x00);
        spiWrite(MCP23S08_IODIR, 0x00);
    } 
    else if (mode == GPIO_INPUT_PULLUP)
    {
        spiWrite(MCP23S08_GPPU, 0xFF);
        spiWrite(MCP23S08_IODIR, 0xFF);
    }
}

bool MCP23S08::digitalWrite(uint8_t pin, uint8_t val) const 
{
    uint8_t data = 0;

    if (pin > 7) return false;

    data = spiRead(MCP23S08_GPIO);
    data = (val == HIGH) ? (data | 1 << pin) : (data & ~(1 << pin));
    spiWrite(MCP23S08_GPIO, data);

    return true;
}

uint8_t MCP23S08::digitalRead(uint8_t pin) const 
{
    if (pin > 7) return 0x00;

    return ((spiRead(MCP23S08_GPIO) >> pin) & 0x01);
}

void MCP23S08::write(uint8_t val) const
{
    spiWrite(MCP23S08_GPIO, val);
}

void MCP23S08::write(uint8_t reg, uint8_t val) const
{
    spiWrite(reg, val);
}

uint8_t MCP23S08::read() const
{
    return spiRead(MCP23S08_GPIO);
}

uint8_t MCP23S08::read(uint8_t reg) const
{
    return spiRead(reg);
}

/**
 * Private: Hardware SPI Write Function
*/
void MCP23S08::spiWrite(uint8_t reg, uint8_t byte) const 
{
    _cs_pin.digitalWrite(false);
    _spi.transfer(_address);
    _spi.transfer(reg);
    _spi.transfer(byte);
    _cs_pin.digitalWrite(true);
}

/**
 * Private: Hardware SPI Read Function
*/
uint8_t MCP23S08::spiRead(uint8_t reg) const 
{
    uint8_t data = 0;

    _cs_pin.digitalWrite(false);
    _spi.transfer(_address | 0x01);
    _spi.transfer(reg);
    data = _spi.transfer(0);
    _cs_pin.digitalWrite(true);

    return data;
}

}

// EOF

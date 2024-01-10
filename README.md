# MCP23S08 IO-Expander Driver

Portable, multi-instance IO-expander driver.

## Overview

This HAL-mediated custom [MCP23S08](https://www.digikey.com/en/products/detail/microchip-technology/MCP23S08T-E-SO/739287) IO-expander driver permits ease of use that is portable across many platforms.

## Usage

The implementation relies on an external user-defined hardware abstraction layer (HAL) called `hal.h` which defines the necessary calls in the `HAL` namespace. Namely, a GPIO pin object with `pinMode()` and `digitalWrite()` methods, and a SPI bus object with `init()` and `transfer()` methods.

The HAL GPIO pin object `pinMode()` method should set as output when supplied with a const uint8_t `GPIO_OUTPUT`. Additionally, the `pinMode()` method of the MCP23S08 requires values `GPIO_INPUT` and `GPIO_INPUT_PULLUP`. The `transfer()` method of the HAL SPI bus should clock out the `uint8_t` value supplied in its argument to the input clock and data pins of the IO-expander.

### Example

```cpp
#include <mcp23s08.h>

...

// Instantiate IO-expander
static const uint8_t    MCP23X08_ADDRESS = 0x20;
PeripheralIO::MCP23S08  spi_io(MCP23X08_ADDRESS, PIN_A2);

...

int main()
{
    uint8_t read_data;

...
    // Init IO-expander
    spi_io.init();
    spi_io.portMode(GPIO_INPUT);

...
    // Read IO-expander on condition
    if (someCondition)
    {
        read_data = spi_io.read();
    }

...

}

...
```

## License

MIT © 2023 John Greenwell
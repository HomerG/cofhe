#include "uart.h"

// #include <iostream> // remove
#include "termite.h"

using namespace std;

namespace e3
{

// Constructors

Uart::Uart(const std::string & portname, int baudrate)
    : portname(portname), baudrate(baudrate)
{
    init();
}

// Private functions

void Uart::init()
{
    auto success = termite::connect(handler, portname, baudrate);
    if (!success) throw "Cannot connect to UART " + portname;
}

// Public functions

void Uart::read(uint8_t * data, uint32_t size)
{
    // cout << "Uart::read\n";
    uint32_t bytesRead = 0;
    do
    {
        auto r = termite::read(handler, &data[bytesRead], size-bytesRead);
        bytesRead += r;
    } while (bytesRead < size);
    // cout << "Uart::done reading " << bytesRead << "\n";
}

void Uart::write(const uint8_t * data, uint32_t size)
{
    // cout << "Uart::write\n";
    auto r = termite::write(handler, data, size);
}

} // e3

#include <iostream> // remove

#include "spi.hpp"

using namespace std;

namespace e3
{

// Constructors

Spi::Spi(const string & portname)
    : portname(portname)
{
    // init
}

// Public functions

void Spi::read(uint8_t * data, uint32_t size)
{
    cout << "Spi::read\n";
}

void Spi::write(const uint8_t * data, uint32_t size)
{
    cout << "Spi::write\n";
}

} // e3

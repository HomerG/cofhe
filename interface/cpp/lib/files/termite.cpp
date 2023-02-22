#include "termite.h"

// for some reason, the order matters, which is a bad thing
// so to use read and write from termios2, this must be included first
#ifdef USE_TERMIOS
#pragma message "Using TERMIOS"
#include "termios.h"
#include "termios2.h"
#elif USE_TERMIOS_ONLY
#pragma message "Using TERMIOS ONLY"
#include "termios.h"
#elif USE_TERMIOS2_ONLY
#pragma message "Using TERMIOS2 ONLY"
#include "termios2.h"
#else
#pragma message "Using TERMIOS2"
#include "termios2.h"
#include "termios.h"
#endif

using namespace std;

namespace termite
{

bool connect(int & handler, const string & portname, int baudrate)
{
#ifdef USE_TERMIOS
    auto success2 = termios2::connect(handler, portname.c_str(), baudrate);
    auto success1 = termios::connect(handler, portname.c_str(), baudrate);
#elif USE_TERMIOS_ONLY
    auto success1 = termios::connect(handler, portname.c_str(), baudrate);
    bool success2 = true;
#elif USE_TERMIOS2_ONLY
    bool success1 = true;
    auto success2 = termios2::connect(handler, portname.c_str(), baudrate);
#else
    auto success1 = termios::connect(handler, portname.c_str(), baudrate);
    auto success2 = termios2::connect(handler, portname.c_str(), baudrate);
#endif
    return success1 && success2;
}

int read(int handler, uint8_t * data, int size)
{
#if defined(USE_TERMIOS) || defined(USE_TERMIOS_ONLY)
    return termios::read(handler, data, size);
#else
    return termios2::read(handler, data, size);
#endif
}

int write(int handler, const uint8_t * data, int size)
{
#if defined(USE_TERMIOS) || defined(USE_TERMIOS_ONLY)
    return termios::write(handler, data, size);
#else
    return termios2::write(handler, data, size);
#endif
}

} // termite

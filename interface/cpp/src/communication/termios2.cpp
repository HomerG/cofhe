#include "termios2.hpp"

namespace termios2
{

bool connect(int & handler, const char * portname, int baudrate)
{
    handler = open(portname, O_RDWR | O_NOCTTY);
    struct termios2 tio;
    ioctl(handler, TCGETS2, &tio);
    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    tio.c_ispeed = baudrate;
    tio.c_ospeed = baudrate;
    return !ioctl(handler, TCSETS2, &tio);
}

} // termios2

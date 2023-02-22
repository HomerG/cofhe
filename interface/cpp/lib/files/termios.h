#pragma once

#include <cstdint>

namespace termios
{

#include <fcntl.h>   /* File Control Definitions           */
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions          */

bool connect(int & handler, const char * portname, int baudrate);

} // termios

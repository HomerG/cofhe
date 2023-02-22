#pragma once

#include <cstdint>

namespace termios2
{

#include <asm/ioctls.h>   // posix
#include <asm/termbits.h> // posix
#include <fcntl.h>        // posix
#include <sys/ioctl.h>    // posix
#include <unistd.h>       // posix

bool connect(int & handler, const char * portname, int baudrate);

} // termios2

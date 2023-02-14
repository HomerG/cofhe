#pragma once

#include <cstdint>
#include <string>

namespace termite
{

bool connect(int & handler, const std::string & portname, int baudrate);
int read(int handler, uint8_t * data, int size);
int write(int handler, const uint8_t * data, int size);

} // termite

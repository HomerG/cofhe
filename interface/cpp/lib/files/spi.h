#pragma once

#include <string>
#include <vector>

#include "communication.h"

namespace e3
{

class Spi : public Communication
{
    private:
        std::string portname;

    public:
        Spi(){}
        Spi(const std::string & portname);

        void read (      uint8_t * data, uint32_t size);
        void write(const uint8_t * data, uint32_t size);
};

} // e3

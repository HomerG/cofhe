#pragma once

#include <vector>

namespace e3
{

class Communication
{
    public:
        virtual void read (      uint8_t * data, uint32_t size) = 0;
        virtual void write(const uint8_t * data, uint32_t size) = 0;
};

} // e3

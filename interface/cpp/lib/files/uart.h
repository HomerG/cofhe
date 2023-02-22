#pragma once

#include <string>
#include <vector>

#include "communication.h"

namespace e3
{

class Uart : public Communication
{
    private:
        std::string portname;
        int baudrate;
        int handler;

        void init();

    public: // FIXME: add destructor to close port
        Uart(){}
        Uart(const std::string & portname, int baudrate);

        void read (      uint8_t * data, uint32_t size);
        void write(const uint8_t * data, uint32_t size);
};

} // e3

#include <string>

#include "spi.hpp"

using namespace e3;
using namespace std;

int main()
{
    string portname = "/dev/ttyUSB1";
    Spi spi;

    spi = Spi(portname);
}

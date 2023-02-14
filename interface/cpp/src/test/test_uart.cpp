#include <string>

#include "uart.hpp"

using namespace e3;
using namespace std;

int main()
{
    string portname = "/dev/ttyUSB1";
    int baudrate = 1000000;
    Uart uart;
    
    uart = Uart(portname, baudrate);
}

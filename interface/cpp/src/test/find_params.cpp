#include <cstdint>
#include <iostream>
#include <map>
#include <string>

#include "math.hpp"
#include "number.h"
#include "ringint.hpp"
#include "support.hpp"

using namespace math;
using namespace std;
using namespace support;

template <class T> int error(const T & e) { cout << "error: " << e << '\n'; return 1; }

int main()
try
{
    int bitlength = 100;
    uint32_t logn = 13;
    uint32_t n = 1 << logn;
    Number k = (Number(1)<<bitlength)/(2*n);
    for (; true; k++)
    {
        MathParams mp;
        try { mp = MathParams(n, k); }
        catch(...) { continue; }
        mp.print();
    }
}
catch (const char   * e) { return error(e); }
catch (const string & e) { return error(e); }

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h> // for sleep

#include "cofhe_const.hpp"
#include "cofhe.hpp"
#include "conversion.hpp"
#include "io.hpp"
#include "math.hpp"
#include "number.h"
#include "ringint.hpp"
#include "support.hpp"

using namespace conversion;
using namespace e3::cofhe;
using namespace io;
using namespace math;
using namespace std;
using namespace support;

template <class T> int error(const T & e) { cout << "error: " << e << '\n'; return 1; }
void testNtt(Cofhe &, uint32_t, uint32_t);

const int HEX_BASE = 16;

int main()
try
{
    map<string,DataType> params;
    params["portname"] = "/dev/ttyUSB1";
    params["baudrate"] = 1000000;
    uint32_t logn = 12;
    uint32_t logq = 128; // bits

    Cofhe cofhe(UART, params);
    testNtt(cofhe, logn, logq);
}
catch (const char   * e) { return error(e); }
catch (const string & e) { return error(e); }

void testNtt(Cofhe & cofhe, uint32_t logn, uint32_t logq)
{
    const int nRoots = 1;
    uint32_t n = 1 << logn;
    auto max = Number(1) << logq;
    for (uint32_t const_mul = 2; true; const_mul+=2) // fixed for 4096
    {
        auto mod = const_mul * ( Number(1) << logn ) + 1;
        cout << "Modulus: " << mod << " (k: " << const_mul << ")\n";
        if ( mod >= max ) break;
        if ( !mod.isPrime() ) continue;
        // Ringint::setModulus(mod);
        // auto invN = Ringint::inv(n);
        auto nInv = Number::inv(n, mod);
        cout << "n inverse: " << nInv << '\n';
        Number nthRoot;
        try { nthRoot = find1NthRoot(n, mod, true); }
        catch (...) { continue; };
        cout << "Selected Nth Root of Unity: " << nthRoot << '\n';

        cout << "Generating polynomial and twiddle factor\n";
        vector<string> strPoly(n);
        vector<string> strTwiddle(n);
        for (uint32_t i=0; i<n; i++)
        {
            strPoly[i]    = Number::random(mod).str(HEX_BASE);
            strTwiddle[i] = Number::powm(nthRoot, i, mod).str(HEX_BASE);
        }

        cout << "Configuring CoFHE\n";
        auto logqBytes = bit2byte(logq);
        auto modulus  = strhextouint8( mod.str(HEX_BASE), logqBytes);
        auto nInverse = strhextouint8(nInv.str(HEX_BASE), logqBytes);
        auto twiddle  = strhextouint8(        strTwiddle, logqBytes);
        Register regTwiddle = R2;
        cofhe.configure(n, logq, modulus, nInverse, twiddle, regTwiddle);

        cout << "Loading polynomial and starting operation\n";
        auto poly = strhextouint8(strPoly, logqBytes);
        // printPair(strPoly, poly, logqBytes);
        cofhe.setRegister(poly, R0);

        auto regNtt = cofhe.ntt(R0, R3);
        auto regCmul = ( regNtt == R0 ? R3 : R0 );

        cofhe.cmul(regCmul, regNtt);
        auto regTmp = regNtt;
        auto regOut = cofhe.intt(regCmul, regTmp);
        sleep(2);

        cout << "Checking result\n";
        auto out = cofhe.getRegister(regOut);
        // auto out = cofhe.getRegister(R0);

        size_t count = 0;
        for ( uint32_t i=0; i < n * bit2byte(logq); i++ )
            if ( poly[i] != out[i] )
            {
                cout << hex << i << " expected " << uint32_t(poly[i]) << " got " << uint32_t(out[i]) << '\n';
                count++;
            }
        cout << "# errors: " << dec << count << "\n\n";
        const_mul++;
    }
}

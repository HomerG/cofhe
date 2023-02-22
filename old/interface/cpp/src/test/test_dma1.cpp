#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h> // for sleep
#include <vector>

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
constexpr bool isOdd(uint32_t n) { return bool(n&1); }
void intt(Cofhe & cofhe, Register & ri, Register & rs, bool waitInterrupt=true);
void ntt(Cofhe & cofhe, Register & ri, Register & rs, bool waitInterrupt=true);
void testCtmul(Cofhe &, uint32_t, uint32_t, bool);

const int HEX_BASE = 16;
enum Baudrate { SLOW, FAST, FASTEST };
const vector<int> BAUDRATE { 9600, 512000, 1440000 };

int main()
try
{
    uint32_t logn  = 13;
    Baudrate baud  = FASTEST;
    uint32_t logq  = 128; // bits
    bool dual      = true; // true for dual-port memory
    bool portOrder = true; // true: 0,1, false 1,0

    map<string,DataType> params;
    params["frequency"] = 24000000; // hz
    params["portname"]      = portOrder ? "/dev/ttyUSB0" : "/dev/ttyUSB1";
    params["portinterrupt"] = portOrder ? "/dev/ttyUSB1" : "/dev/ttyUSB0";
    params["baudrate"] = BAUDRATE[baud];

    Cofhe cofhe(UART, params);
    testCtmul(cofhe, logn, logq, dual);
}
catch (const char   * e) { return error(e); }
catch (const string & e) { return error(e); }

void intt(Cofhe & cofhe, Register & ri, Register & rs, bool waitInterrupt)
{
    cofhe.cmul(ri, ri, false);
    auto r = cofhe.intt(ri, rs, waitInterrupt);
    if (r != ri) std::swap(ri, rs);
}

void ntt(Cofhe & cofhe, Register & ri, Register & rs, bool waitInterrupt)
{
    auto r = cofhe.ntt(ri, rs, waitInterrupt);
    if (r != ri) std::swap(ri, rs);
}

void testCtmul(Cofhe & cofhe, uint32_t logn, uint32_t logq, bool dual)
{
    uint32_t n = 1 << logn;
    auto logqBytes = bit2byte(logq);

    auto maxQ = Number(1) << logq;
    for (Number k="11534531126405109766848"; true; k++)
    {
        cout << "\nFinding parameters for k = " << k << '\n';
        MathParams mp;
        try { mp = MathParams(n, k); }
        catch (const char * e) { cout << e << '\n'; continue; }
        if (mp.q >= maxQ) break;
        auto twiddle = toUint8(mp.twiddle, logqBytes);
        mp.print();

        cout << "\nGenerating polynomials .. " << flush;
        auto p = randomRingintPolynomial(n);
        cout << "ok\n";

        cout << "\nPrecomputing polynomials .. " << flush;
        auto poly = toUint8(p, logqBytes);
        cout << "ok\n";

        cout << "\nConfiguring CoFHE .. " << flush;
        auto modulus    = toUint8(mp.q , logqBytes);
        auto nInverse   = toUint8(mp.in, logqBytes);
        auto barrett_md = toUint8(mp.barrett_md, logqBytes<<1);
        Register rt = R6;
        cofhe.configure(logn, logq, modulus, nInverse, mp.barrett_2k, barrett_md, twiddle, rt, dual);
        cout << "ok\n";

        cout << "DMA transfer .. " << flush;
        cofhe.setRegister(poly, R0);
        // cofhe.setRegister(poly, R5);
        // Register regOut = R5;
        auto regOut = cofhe.move(R0, R5, false);
        sleep(2);
        auto polyR0  = cofhe.getRegister(R0);
        auto polyOut = cofhe.getRegister(regOut);
        cout << "ok\n";

        cout << "\nPoscomputing polynomials .. " << flush;
        auto pOut = toRingint(polyOut, n, logqBytes);
        cout << "ok\n";

        cout << "\nChecking result .. " << flush;
        Polynomial<Ringint> pr(p);
        Polynomial<Ringint> prOut(pOut);

        auto passed = pr == prOut;
        cout << ( passed ? "passed" : "failed") << '\n';
        if ( !passed )
        {
            for ( size_t i=0; i<n; i++ )
                cout << hex << uint32_t( polyOut[i] ) << ":" << uint32_t( polyR0[i] ) << ' ';
            cout << '\n';
        }
    }
}

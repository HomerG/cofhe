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
void testNtt(Cofhe &, uint32_t, uint32_t, bool);

const int HEX_BASE = 16;
enum Baudrate { SLOW, FAST, FASTEST };
const vector<int> BAUDRATE { 9600, 512000, 1440000 };

int main()
try
{
    Number::setPrimeLevel(2);
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
    testNtt(cofhe, logn, logq, dual);
}
catch (const char   * e) { return error(e); }
catch (const string & e) { return error(e); }

void testNtt(Cofhe & cofhe, uint32_t logn, uint32_t logq, bool dual)
{
    uint32_t n = 1 << logn;
    auto logqBytes = bit2byte(logq);

    auto maxQ = Number(1) << logq;
    for (Number k=1; true; k++)
    {
        cout << "\nFinding parameters for k = " << k << '\n';
        MathParams mp;
        try { mp = MathParams(n, k); }
        catch (const char * e) { cout << e << '\n'; continue; }
        if (mp.q >= maxQ) break;
        auto twiddle = toUint8(mp.twiddle, logqBytes);
        mp.print();

        cout << "\nGenerating polynomial .. " << flush;
        auto pA = randomRingintPolynomial(n);
        cout << "ok\n";

        cout << "\nConfiguring CoFHE .. " << flush;
        auto modulus  = toUint8(mp.q , logqBytes);
        auto nInverse = toUint8(mp.in, logqBytes);
        auto barrett_md = toUint8(mp.barrett_md, logqBytes<<1);
        Register regTwiddle = R3;
        cofhe.configure(logn, logq, modulus, nInverse, mp.barrett_2k, barrett_md, twiddle, regTwiddle, dual);
        cout << "ok\n";

        cout << "\nLoading polynomial .. " << flush;
        auto poly = toUint8(pA, logqBytes);
        // printPair(strPoly, poly, logqBytes);
        cofhe.setRegister(poly, R0);
        cout << "ok\n";

        cout << "\nStarting operation .. " << flush;
        auto regNtt = cofhe.ntt(R0, R1, true);
        auto regCmul = ( regNtt == R0 ? R1 : R0 );

        cofhe.cmul(regCmul, regNtt, true);
        auto regTmp = regNtt;
        auto regOut = cofhe.intt(regCmul, regTmp, true);
        // sleep(2);
        cout << "ok\n";

        cout << "\nChecking result .. ";
        auto polyOut = cofhe.getRegister(regOut);
        auto pOut = toRingint(polyOut, n, logqBytes);

        if ( pA.size() != pOut.size() )
            cout << "failed (polynomial size)\n";
        else
        {
            size_t count = 0;
            for ( int i=0; i < pA.size(); i++ )
                if ( pA[i] != pOut[i] )
                {
                    cout << hex << "( " << i << ", " << pA[i] << " != " << pOut[i] << ") " << flush;
                    count++;
                }
            if (count)
            {
                cout << "\nfailed (# errors: " << dec << count << ")\n";
                return;
            }
            else cout << "passed\n";
        }
    }
}

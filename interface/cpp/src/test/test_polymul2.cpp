// 109-bit q

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
void testNtt(Cofhe &, uint32_t, uint32_t, bool);

const int HEX_BASE = 16;

int main()
try
{
    map<string,DataType> params;
    params["portname"] = "/dev/ttyUSB0";
    // params["portname"] = "/dev/ttyUSB1";
    params["baudrate"] = 9600;
    // params["baudrate"] = 1000000;
    uint32_t logn = 13; //12;
    uint32_t logq = 128; // bits
    bool dual = true; // true for dual-port memory

    Cofhe cofhe; //(UART, params);
    testNtt(cofhe, logn, logq, dual);
}
catch (const char   * e) { return error(e); }
catch (const string & e) { return error(e); }

void testNtt(Cofhe & cofhe, uint32_t logn, uint32_t logq, bool dual)
{
    cout << "Running...\n";
    uint32_t n = 1 << logn;
    auto logqBytes = bit2byte(logq);
    auto maxQ = Number(1) << logq;
    for (Number k = (Number(1)<<32)/(2*n); true; k++)
    {
        // cout << "\nFinding parameters for k = " << k << '\n';
        MathParams mp;
        try { mp = MathParams(n, k); }
        catch (const char * e) { /*cout << e << '\n';*/ continue; }
        if (mp.q >= maxQ) break;
        auto twiddle = toUint8(mp.twiddle, logqBytes);
        mp.print();
        continue;

        cout << "\nGenerating polynomials .. " << flush;
        auto pA = randomRingintPolynomial(n);
        auto pB = randomRingintPolynomial(n);
        cout << "ok\n";

        cout << "\nPrecomputing polynomials .. " << flush;
        auto polyA = toUint8(hadamard(pA, mp.psi), logqBytes);
        auto polyB = toUint8(hadamard(pB, mp.psi), logqBytes);
        cout << "ok\n";

        cout << "\nConfiguring CoFHE .. " << flush;
        auto modulus    = toUint8(mp.q , logqBytes);
        auto nInverse   = toUint8(mp.in, logqBytes);
        auto barrett_md = toUint8(mp.barrett_md, logqBytes<<1);
        Register regTwiddle = R3;
        cofhe.configure(n, logq, modulus, nInverse, mp.barrett_2k, barrett_md, twiddle, regTwiddle);
        cout << "ok\n";

        cout << "\nPolynomial multiplication .. " << flush;
        auto polyOut = cofhe.polymul(polyA, polyB, R0, R1, R2, dual);
        cout << "ok\n";

        cout << "\nPoscomputing polynomial .. " << flush;
        auto pOut = hadamard(toRingint(polyOut, n, logqBytes), mp.ipsi);
        cout << "ok\n";

        cout << "\nChecking result .. " << flush;
        Polynomial<Ringint> prA(pA);
        Polynomial<Ringint> prB(pB);
        Polynomial<Ringint> prOut(pOut);
        Polynomial<Ringint> prMod; // x^n + 1
        prMod[0] = 1;
        prMod[n] = 1;
        auto prExp = (prA * prB) % prMod;
        cout << (prOut == prExp ? "passed" : "failed") << '\n';
    }
}

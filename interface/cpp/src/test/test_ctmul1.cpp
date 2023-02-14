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
    uint32_t logn  = 8; //12;
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
    for (Number k=1; true; k++)
    {
        cout << "\nFinding parameters for k = " << k << '\n';
        MathParams mp;
        try { mp = MathParams(n, k); }
        catch (const char * e) { cout << e << '\n'; continue; }
        if (mp.q >= maxQ) break;
        auto twiddle = toUint8(mp.twiddle, logqBytes);
        mp.print();

        cout << "\nGenerating polynomials .. " << flush;
        auto pA0 = randomRingintPolynomial(n);
        auto pA1 = randomRingintPolynomial(n);
        auto pB0 = randomRingintPolynomial(n);
        auto pB1 = randomRingintPolynomial(n);
        cout << "ok\n";

        cout << "\nPrecomputing polynomials .. " << flush;
        auto polyA0 = toUint8(hadamard(pA0, mp.psi), logqBytes);
        auto polyA1 = toUint8(hadamard(pA1, mp.psi), logqBytes);
        auto polyB0 = toUint8(hadamard(pB0, mp.psi), logqBytes);
        auto polyB1 = toUint8(hadamard(pB1, mp.psi), logqBytes);
        cout << "ok\n";

        cout << "\nConfiguring CoFHE .. " << flush;
        auto modulus    = toUint8(mp.q , logqBytes);
        auto nInverse   = toUint8(mp.in, logqBytes);
        auto barrett_md = toUint8(mp.barrett_md, logqBytes<<1);
        Register rt = R6;
        cofhe.configure(logn, logq, modulus, nInverse, mp.barrett_2k, barrett_md, twiddle, rt, dual);
        cout << "ok\n";

        cout << "\nCiphertext multiplication .. " << flush;
        Register a0 = R0, a1 = R3, b0 = R2, b1 = R5, s = R1, r;
        cofhe.setRegister(polyA0, a0);
        cofhe.setRegister(polyA1, a1);
        cofhe.setRegister(polyB0, b0);
        cofhe.setRegister(polyB1, b1);

        // ntt(B0)
        ntt(cofhe, b0, s, false);

        // ntt(A0), move(b0, R4)
        ntt(cofhe, a0, s, false);
        r = cofhe.move(b0, R4, false);

        // mul(y0, a0, b0)
        Register y0 = s;
        cofhe.mul(y0, a0, b0, false);

        // intt(y0)
        s = b0;
        b0 = r;
        intt(cofhe, y0, s, true); // true

        // move(B1, s)
        b1 = cofhe.move(b1, s, false);

        // move(y0, R5)
        s = y0;
        y0 = cofhe.move(y0, R5, true); // true

        // ntt(B1)
        ntt(cofhe, b1, s, true); // true

        // mul(y01, a0, b1), move(a1, s)
        auto y01 = a1;
        a1 = cofhe.move(a1, s, true);

        // move(y01, R3)
        s = a0;
        cofhe.mul(y01, a0, b1, false);

        // ntt(A1)
        ntt(cofhe, a1, s, false);

        // mul(y2, a1, b1)
        auto y2 = s;
        cofhe.mul(y2, a1, b1, false);

        // mul(y10, a1, b0)
        auto y10 = a1;
        cofhe.mul(y10, a1, b0, false);

        // intt(y2)
        s = b1;
        intt(cofhe, y2, s, false);

        // mul(y1, y01, y10)
        auto y1 = y10;
        cofhe.mul(y1, y01, y10, false);

        // intt(y1)
        intt(cofhe, y1, s, true);

        auto polyY0 = cofhe.getRegister(y0);
        auto polyY1 = cofhe.getRegister(y1);
        auto polyY2 = cofhe.getRegister(y2);
        cout << "ok\n";

        cout << "\nPoscomputing polynomials .. " << flush;
        auto pY0 = hadamard(toRingint(polyY0, n, logqBytes), mp.ipsi);
        auto pY1 = hadamard(toRingint(polyY1, n, logqBytes), mp.ipsi);
        auto pY2 = hadamard(toRingint(polyY2, n, logqBytes), mp.ipsi);
        cout << "ok\n";

        cout << "\nChecking result .. " << flush;
        Polynomial<Ringint> prA0(pA0);
        Polynomial<Ringint> prA1(pA1);
        Polynomial<Ringint> prB0(pB0);
        Polynomial<Ringint> prB1(pB1);
        Polynomial<Ringint> prY0(pY0);
        Polynomial<Ringint> prY1(pY1);
        Polynomial<Ringint> prY2(pY2);
        Polynomial<Ringint> prMod; // x^n + 1
        prMod[0] = 1;
        prMod[n] = 1;
        auto prV0 = (prA0 * prB0) % prMod;
        auto prV1 = ( (prA0 * prB1) + (prA1 * prB0) ) % prMod;
        auto prV2 = (prA1 * prB1) % prMod;
        auto f0 = prV0 == prY0;
        auto f1 = prV1 == prY1;
        auto f2 = prV2 == prY2;
        cout << (f0 && f1 && f2 ? "passed" : "failed") << " [" << f0 << f1 << f2 << "]" << '\n';
    }
}

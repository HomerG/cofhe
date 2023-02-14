#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "cofhe.hpp"
#include "conversion.hpp"
#include "io.hpp"
#include "math.hpp"

using namespace conversion;
using namespace e3;
using namespace e3::cofhe;
using namespace io;
using namespace math;
using namespace std;

template <class T> int error(const T & e) { cout << "error: " << e << '\n'; return 1; }
void polymul(Cofhe & cofhe);

int main()
try
{
    map<string,DataType> params;
    params["portname"] = "/dev/ttyUSB1";
    params["baudrate"] = 1000000;

    cout << "Initializing .. " << flush;
    Cofhe cofhe(UART, params);
    cout << "ok\n";

    polymul(cofhe);
}
catch (const char   * e) { return error(e); }
catch (const string & e) { return error(e); }

void polymul(Cofhe & cofhe)
{
    string filename = "_ntt_3_f.log";

    cout << "Loading .. " << flush;
    auto data = load(filename);
    cout << "ok\n";

    cout << " " << data.size() << " x [ " << flush;
    for ( size_t i=0; i<data.size(); i++ ) cout << data[i].size() << " ";
    cout << "]\n";

    cout << "Converting .. " << flush;
    uint32_t n    = 4096;
    uint32_t logn = 14;
    uint32_t logq = 128;
    uint64_t cmul = 3;
    size_t nroots = 1;
    uint64_t inv  = cmul * (1 << logn) + 1;

    // auto ninv     = uint64touint8( inv );
    auto ninv     = uint64touint8 ( modInverse( data[0][0], data[0][1] ) );
    auto modulus  = uint64touint8 ( data[0][1] );
    auto poly     = uint64touint8 ( data[1]    );
    // auto twiddle  = uint64touint8 ( data[2]    );
    auto twiddle = uint64touint8( reverseTwiddle(data[2]) );
    // auto itwiddle = uint64touint8 ( data[3]    ); // can be ignored
    auto ntt      = uint64touint8 ( data[4]    );
    auto mod      = uint8touint64(modulus);
    cout << "proots " << flush;
    auto proots   = primetiveRoots(mod, cmul, nroots);
    cout << "ctwiddle " << flush;
    auto ctwiddle = calcTwiddle(proots[0], mod, n);
    cout << "ok\n";

    size_t logqBytes = bit2byte(logq);
    size_t size = n * logqBytes;
    // print(poly, size, logqBytes);

    size_t count = 0;
    cout << "Checking twiddle .. " << flush;
    auto nbytes = n * bit2byte(logq);
    for ( size_t i=0; i < nbytes; i++ )
        if ( twiddle[i] != ctwiddle[i] ) count++;
    cout << ( count ? "ok" : "failed" ) << '\n';

    cout << "Configuring .. " << flush;
    Register regTwiddle = R2;
    cofhe.configure(n, logq, modulus, ninv, twiddle, regTwiddle);
    cout << "ok\n";

    cout << "Loading polynomial .. " << flush;
    cofhe.setRegister(poly, R0);
    // cofhe.setRegister(ntt, R0);
    cout << "ok\n";

    cout << "NTT .. " << flush;
    auto regNtt = cofhe.ntt(R0, R3);
    cout << "ok\n";

    cout << "cmul .. " << flush;
    auto regCmul = ( regNtt == R0 ? R3 : R0 );
    cofhe.cmul(regCmul, regNtt);
    cout << "ok\n";
    // auto regCmul = regNtt;
    // regNtt = ( regNtt == R0 ? R3 : R0 );
    //
    cout << "intt .. " << flush;
    auto regTmp = regNtt;
    auto regIntt = cofhe.intt(regCmul, regTmp);
    cout << "ok\n";

    cout << "Getting result .. " << flush;
    auto regOut = regIntt;
    auto out = cofhe.getRegister(regOut);
    cout << "ok\n";

    count = 0;
    size_t limit = logqBytes << 4;
    for ( size_t i=0; i < size; i++ )
    {
        // if ( i<limit ) cout << hex << uint32_t(out[i]) << ":" << uint32_t(ntt[i]) << '\n';
        // if ( out[i] != ntt[i] ) count++;
        if ( i<limit ) cout << hex << uint32_t(out[i]) << ":" << uint32_t(poly[i]) << ( (i+1) % logqBytes ? ' ' : '\n' );
        if ( out[i] != poly[i] ) count++;
    }
    cout << dec << "count: " << count << '\n';
}

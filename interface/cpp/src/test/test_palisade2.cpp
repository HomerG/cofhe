#include <fstream>
#include <gmp.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "cofhe.hpp"

using namespace e3;
using namespace e3::cofhe;
using namespace std;

vector<uint64_t> calcTwiddle(uint64_t nth_rou, uint64_t modulus, uint32_t n);
uint8_t charhex2uint8(char c);
template <class T> int error(const T & e) { cout << "error: " << e << '\n'; return 1; }
vector<vector<uint64_t>> load(const string & filename);
uint64_t modInverse(uint64_t number, uint64_t modulus);
vector<uint64_t> primetiveRoots(uint64_t modulus, uint64_t ninv, uint64_t nroots);
void polymul(Cofhe & cofhe);
void print(const uint8_t * din, size_t size, size_t divider=0);
vector<string> read(const string & filename);
uint8_t   strhexbytetouint8(const string & s);
uint8_t * strhextouint8(const string & s);
uint8_t * strhextouint8(const vector<string> & v);
uint8_t * uint64touint8(uint64_t din);
uint8_t * uint64touint8(const vector<uint64_t> & din);
uint64_t  uint8touint64(const uint8_t * n);

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

vector<uint64_t> calcTwiddle(uint64_t nth_rou, uint64_t modulus, uint32_t n)
{
    vector<uint64_t> twiddle;
    mpz_t base, mod;
    mpz_init_set_ui(base, nth_rou);
    mpz_init_set_ui(mod, modulus);
    for ( uint32_t i=0; i<n; i++ )
    {
        mpz_t e;
        mpz_init(e);
        mpz_powm_ui(e, base, i, mod);
        twiddle.push_back( mpz_get_ui(e) );
    }
    return twiddle;
}

uint8_t charhex2uint8(char c)
{
    switch (c)
    {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'a': return 10;
        case 'b': return 11;
        case 'c': return 12;
        case 'd': return 13;
        case 'e': return 14;
        case 'f': return 15;
    }
    throw "Invalid hexadecimal during conversion";
}

vector<vector<uint64_t>> load(const string & filename)
{
    vector<vector<uint64_t>> data;
    ifstream fin(filename);
    for( string line; getline(fin, line); )
    {
        vector<uint64_t> v;
        stringstream ss(line);
        for ( string item; ss >> item; ) v.push_back( uint64_t( stoull(item) ) );
        data.push_back(v);
    }
    return data;
}

uint64_t modInverse(uint64_t number, uint64_t modulus)
{
    mpz_t n, m, inv;
    mpz_init(inv);
    mpz_init_set_ui(n, number);
    mpz_init_set_ui(m, modulus);
    mpz_invert(inv, n, m);
    gmp_printf("%Zx %Zx %Zx\n", n, m, inv);
    return uint64_t( mpz_get_ui(inv) );
}

vector<uint64_t> primetiveRoots(uint64_t modulus, uint64_t cmul, size_t nroots)
{
    mpz_t mod;
    mpz_init_set_ui(mod, modulus);
    vector<uint64_t> roots;
    size_t hit = 0;
    for ( uint64_t g=1; g<modulus; g++ )
    {
        if ( hit == nroots ) break;
        mpz_t base;
        mpz_init_set_ui(base, g);
        for ( uint64_t powers=1; powers<modulus; powers++ )
        {
            mpz_t powmod;
            mpz_init(powmod);
            mpz_powm_ui(powmod, base, powers, mod);
            if ( mpz_get_ui(powmod) == 1 )
            {
                if ( powers == modulus-1 )
                {
                    mpz_t root;
                    mpz_init(root);
                    mpz_pow_ui(root, base, cmul);
                    if ( mpz_cmp(root, mod) < 0 )
                    {
                        roots.push_back( mpz_get_ui(root) );
                        hit++;
                    }
                    mpz_clear(root);
                }
                else break;
            }
            mpz_clear(powmod);
        }
        mpz_clear(base);
    }
    return roots;
}

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
    auto twiddle  = uint64touint8 ( data[2]    );
    // auto itwiddle = uint64touint8 ( data[3]    ); // can be ignored
    auto ntt      = uint64touint8 ( data[4]    );
    auto mod      = uint8touint64(modulus);
    // auto proots   = primetiveRoots(mod, cmul, nroots);
    // auto ctwiddle = calcTwiddle(proots[0], mod, n);
    cout << "ok\n";

    size_t logqBytes = bit2byte(logq);
    size_t size = n * logqBytes;
    // print(poly, size, logqBytes);

    size_t count = 0;
    // cout << "Checking twiddle .. " << flush;
    // auto nbytes = n * bit2byte(logq);
    // for ( size_t i=0; i < nbytes; i++ )
    //     if ( twiddle[i] != ctwiddle[i] ) count++;
    // cout << ( count ? "ok" : "failed" ) << '\n';

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

void print(const uint8_t * din, size_t size, size_t divider)
{
    cout << hex;
    for ( size_t i=0; i<size; )
    {
        cout << uint16_t( din[i++] );
        cout << ( divider && ( i % divider ) ? ' ' : '\n' );
    }
    cout << dec;
}

vector<string> read(const string & filename)
{
    vector<string> vout;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) ) vout.push_back(line);
    return vout;
}

uint8_t strhexbytetouint8(const string & s)
{
    return ( charhex2uint8( s[0] ) << 4 ) | charhex2uint8( s[1] );
}

uint8_t * strhextouint8(const string & s)
{
    auto bytesize = 2;
    string str = s.substr(2);
    vector<string> tmp;
    for ( int begin = str.size()-bytesize; begin >= 0; begin -= bytesize )
        tmp.push_back( str.substr(begin, bytesize) );

    auto nbytes = tmp.size();
    auto size = sizeof(uint8_t) * nbytes;
    uint8_t * data = (uint8_t *) malloc(size);
    size_t i = 0;
    for ( auto & b : tmp )
        data[i++] = strhexbytetouint8(b);
    return data;

}

uint8_t * strhextouint8(const vector<string> & v)
{
    // remove 0x, break into bytes, and convert to little endian
    vector<vector<string>> m;
    auto bytesize = 2;
    for ( const auto & e : v )
    {
        string str = e.substr(2);
        vector<string> tmp;
        for ( int begin = str.size()-bytesize; begin >= 0; begin -= bytesize )
            tmp.push_back( str.substr(begin, bytesize) );
        m.push_back(tmp);
    }

    // convert o uint8_t *
    auto ncoeff = m.size();
    auto nbytes = m[0].size();
    auto size = sizeof(uint8_t) * ncoeff * nbytes;
    uint8_t * data = (uint8_t *) malloc(size);
    size_t i = 0;
    for ( auto & vn : m )
    {
        for ( auto & b : vn )
            data[i++] = strhexbytetouint8(b);
    }
    return data;
}

uint8_t * uint64touint8(uint64_t din)
{
    auto s    = sizeof(uint64_t);
    auto size = s << 1;
    auto dout = (uint8_t *) malloc(size);
    size_t i;
    for ( i=0; i<s   ; i++ ) dout[i] = ( din >> byte2bit(i) ) & 0xff;
    for (    ; i<size; i++ ) dout[i] = 0;
    return dout;
}

uint8_t * uint64touint8(const vector<uint64_t> & din)
{
    auto s    = sizeof(uint64_t);
    auto size = s << 1;
    auto dout = (uint8_t *) malloc( size * din.size() );
    for (size_t j=0; j<din.size(); j++)
    {
        size_t i;
        for ( i=0; i<s   ; i++ ) dout[ j*size + i ] = ( din[j] >> byte2bit(i) ) & 0xff;
        for (    ; i<size; i++ ) dout[ j*size + i ] = 0;
    }
    return dout;
}

uint64_t  uint8touint64(const uint8_t * n)
{
    uint64_t r = 0;
    for ( size_t i = sizeof(uint64_t) / sizeof(uint8_t); i > 0; i-- )
        r |= uint64_t( n[i-1] ) << byte2bit(i-1);
    return r;
}

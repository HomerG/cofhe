#include <fstream>
#include <gmp.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cofhe_const.hpp"
#include "cofhe.hpp"

using namespace e3;
using namespace e3::cofhe;
using namespace std;

vector<uint64_t> calcTwiddle(uint64_t nth_rou, uint64_t modulus, uint32_t n);
uint8_t charhex2uint8(char c);
template <class T> int error(const T & e) { cout << "error: " << e << '\n'; return 1; }
vector<uint64_t> primetiveRoots(uint64_t modulus, uint64_t ninv, uint64_t nroots);
void polymul(Cofhe & cofhe);
vector<string> read(const string & filename);
uint8_t   strhexbytetouint8(const string & s);
uint8_t * strhextouint8(const string & s);
uint8_t * strhextouint8(const vector<string> & v);
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
                }
                else break;
            }
        }
    }
    return roots;
}

void polymul(Cofhe & cofhe)
{
    string root        = "../python/";
    string filepoly1   = root + "fseq.data";
    string filepoly2   = root + "fseq2.data";
    string filetwiddle = root + "ftwdl.data";
    string fileparams  = root + "params.data";
    string fileresult  = root + "polymul.data";

    cout << "Loading .. " << flush;
    auto strpoly1   = read(filepoly1);
    auto strpoly2   = read(filepoly2);
    auto strtwiddle = read(filetwiddle);
    auto strparams  = read(fileparams);
    auto strresult  = read(fileresult);
    cout << "ok\n";

    cout << "Converting .. " << flush;
    uint32_t n    = 4096;
    uint32_t logq = 128;
    uint64_t cmul = 3;
    size_t nroots = 1;
    auto modulus  = strhextouint8(strparams[0]);
    auto ninv     = strhextouint8(strparams[1]);
    auto poly1    = strhextouint8(strpoly1);
    auto poly2    = strhextouint8(strpoly2);
    auto twiddle  = strhextouint8(strtwiddle);
    auto result   = strhextouint8(strresult);
    auto mod      = uint8touint64(modulus);
    auto inv      = uint8touint64(ninv);
    auto proots   = primetiveRoots(mod, cmul, nroots);
    auto ctwiddle = uint64touint8( calcTwiddle(proots[0], mod, n) );
    cout << "ok\n";

    cout << "Primitive roots:" << flush;
    for ( auto root : proots ) cout << ' ' << root << flush;
    cout << '\n';

    cout << "Checking twiddle .. " << flush;
    size_t count = 0;
    auto nbytes = n * bit2byte(logq);
    for ( size_t i=0; i < nbytes; i++ )
        if ( twiddle[i] != ctwiddle[i] ) count++;
    cout << ( count ? "failed" : "ok" ) << '\n';

    cout << "Configuring .. " << flush;
    Register regTwiddle = R2;
    cofhe.configure(n, logq, modulus, ninv, twiddle, regTwiddle);
    cout << "ok\n";

    cout << "Polynomial multiplication .. " << flush;
    auto out = cofhe.polymul(poly1, poly2, R0, R1, R3);
    cout << "ok\n";

    // size_t count = 0;
    // auto nbytes = n * bit2byte(logq);
    count = 0;
    for ( size_t i=0; i < nbytes; i++ )
    {
        cout << uint32_t(out[i]) << ":" << uint32_t(result[i]) << " ";
        if ( out[i] != result[i] ) count++;
    }
    cout << "\ncount: " << count << '\n';
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

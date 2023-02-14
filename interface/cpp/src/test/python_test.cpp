#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "cofhe_const.hpp"
#include "cofhe.hpp"

using namespace e3;
using namespace e3::cofhe;
using namespace std;

uint8_t charhex2uint8(char c);
template <class T> int error(const T & e) { cout << "error: " << e << '\n'; return 1; }
vector<string> read(const string & filename);
void polymul(Cofhe & cofhe);
uint8_t   strhexbyte2uint8(const string & s);
uint8_t * strhex2uint8(const string & s);
uint8_t * strhex2uint8(const vector<string> & v);

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
    auto modulus  = strhex2uint8(strparams[0]);
    auto ninv     = strhex2uint8(strparams[1]);
    auto poly1    = strhex2uint8(strpoly1);
    auto poly2    = strhex2uint8(strpoly2);
    auto twiddle  = strhex2uint8(strtwiddle);
    auto result   = strhex2uint8(strresult);
    cout << "ok\n";

    cout << "Configuring .. " << flush;
    Register regTwiddle = R2;
    cofhe.configure(n, logq, modulus, ninv, twiddle, regTwiddle);
    cout << "ok\n";

    cout << "Polynomial multiplication .. " << flush;
    auto out = cofhe.polymul(poly1, poly2, R0, R1, R3);
    cout << "ok\n";

    size_t count = 0;
    auto nbytes = n * bit2byte(logq);
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

uint8_t strhexbyte2uint8(const string & s)
{
    return ( charhex2uint8( s[0] ) << 4 ) | charhex2uint8( s[1] );
}

uint8_t * strhex2uint8(const string & s)
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
        data[i++] = strhexbyte2uint8(b);
    return data;

}

uint8_t * strhex2uint8(const vector<string> & v)
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
            data[i++] = strhexbyte2uint8(b);
    }
    return data;
}

#include "conversion.h"

#include "cofhe_const.h"

using namespace e3::cofhe;
using namespace std;

namespace conversion
{

const int HEX_BASE = 16;
const string HEX_PREFIX = "0x";

uint8_t charhextouint8(char c)
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
    throw "Invalid hexadecimal during conversion from char to uint8_t";
}

string fillhexstr(const string & s, int nChars)
{
    string str = removeHexPrefix(s);
    int diff = s.size() - nChars;
    string prefix = isHexPrefix(s) ? HEX_PREFIX : "";
    return prefix + ( diff > 0 ? str.substr(diff) : strRepeat('0', -diff) + str );
}

bool isHexPrefix(const string & s)
{
    return s.size() > 2 && s[0] == '0' && s[1] == 'x';
}

string removeHexPrefix(const string & s)
{
    return isHexPrefix(s) ? s.substr(2) : s;
}

uint8_t strhexbytetouint8(const string & s)
{
    return ( charhextouint8( s[0] ) << 4 ) | charhextouint8( s[1] );
}

uint8_t * strhextouint8(const string & s)
{
    auto bytesize = 2;
    // string str = s.substr(2);
    string str = removeHexPrefix(s);
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

uint8_t * strhextouint8(const string & s, int nBytes)
{
    return strhextouint8( fillhexstr(s, nBytes<<1) );
}

uint8_t * strhextouint8(const vector<string> & v)
{
    // remove 0x, break into bytes, and convert to little endian
    vector<vector<string>> m;
    auto bytesize = 2;
    for ( const auto & e : v )
    {
        string str = removeHexPrefix(e);
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

uint8_t * strhextouint8(const vector<string> & v, int nBytes)
{
    auto nChars = nBytes << 1;
    vector<string> vstr;
    for (auto & s : v) vstr.push_back( fillhexstr(s, nChars) );
    return strhextouint8(vstr);
}

string strRepeat(char c, int n)
{
    string s;
    while (n-- > 0) s += c;
    return s;
}

Number toNumber(const uint8_t * n, int nBytes)
{
    return Number( uint8tostrhex(n, nBytes), HEX_BASE );
}

vector<Number> toNumber(const uint8_t * n, int nItems, int nBytes)
{
    auto vstr = uint8tostrhex(n, nItems, nBytes);
    vector<Number> v;
    for (auto & s: vstr) v.push_back( Number(s, HEX_BASE) );
    return v;
}

Ringint toRingint(const uint8_t * n, int nBytes)
{
    return Ringint( toNumber(n, nBytes) );
}

vector<Ringint> toRingint(const uint8_t * n, int nItems, int nBytes)
{
    auto vn = toNumber(n, nItems, nBytes);
    vector<Ringint> vr;
    for (auto & n : vn) vr.push_back( Ringint(n) );
    return vr;
}

uint8_t * toUint8(const Number & n, int nBytes)
{
    return strhextouint8(n.str(HEX_BASE), nBytes);
}

uint8_t * toUint8(const Ringint & n, int nBytes)
{
    return strhextouint8(n.str(HEX_BASE), nBytes);
}

uint8_t * toUint8(const vector<Number> & v, int nBytes)
{
    vector<string> vstr;
    for (auto & e : v) vstr.push_back(e.str(HEX_BASE));
    return strhextouint8(vstr, nBytes);
}

uint8_t * toUint8(const vector<Ringint> & v, int nBytes)
{
    vector<string> vstr;
    for (auto & e : v) vstr.push_back(e.str(HEX_BASE));
    return strhextouint8(vstr, nBytes);
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

char uint8tocharhex(uint8_t n)
{
    if (n >= 0 && n <= 9) return char(n + '0');
    if (n >= 10 && n <= 15) return char(n + 'a' - 10);
    throw "Invalid hexadecimal during conversion from uint8_t to char";
}

// one uint8_t * number to string
string uint8tostrhex(const uint8_t * n, int nBytes)
{
    string hexstr;
    for (int i=nBytes-1; i>=0; i--)
    {
        uint8_t hi = *(n + i) >> 4;
        uint8_t lo = *(n + i) & 0xf;
        hexstr += uint8tocharhex(hi);
        hexstr += uint8tocharhex(lo);
    }
    return hexstr;
}

vector<string> uint8tostrhex(const uint8_t * n, int nItems, int nBytes)
{
    vector<string> v;
    for (int i=0; i < nItems; i++)
        v.push_back( uint8tostrhex(&*(n+nBytes*i), nBytes) );
    return v;
}

uint64_t  uint8touint64(const uint8_t * n)
{
    uint64_t r = 0;
    for ( size_t i = sizeof(uint64_t) / sizeof(uint8_t); i > 0; i-- )
        r |= uint64_t( n[i-1] ) << byte2bit(i-1);
    return r;
}

} // conversion

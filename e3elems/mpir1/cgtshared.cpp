#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

#include "cgtshared.h"

using std::cout;
using std::string;


// *** begin cpp_share/util/def_mpir.inc
namespace e3
{

string Bigun::str() const { return p->str(); }

Bigun::Bigun(unsigned long long x): p(new BigunNative(x)) {}
Bigun::Bigun(const Bigun & a) : Bigun() { *p = *a.p; }
Bigun::Bigun(Bigun && a) noexcept : p(nullptr)  { p.swap(a.p); }
Bigun & Bigun::operator=(const Bigun & a) { *p = *a.p; return *this; }
Bigun & Bigun::operator=(Bigun && a) noexcept { p.swap(a.p); return *this; }


Bigun & Bigun::operator+=(const Bigun & a) { *p += *a.p; return *this; }
Bigun & Bigun::operator-=(const Bigun & a) { *p -= *a.p; return *this; }
Bigun & Bigun::operator*=(const Bigun & a) { *p *= *a.p; return *this; }
Bigun & Bigun::operator/=(const Bigun & a) { *p /= *a.p; return *this; }
Bigun & Bigun::operator%=(const Bigun & a) { *p %= *a.p; return *this; }
Bigun & Bigun::operator|=(const Bigun & a) { *p |= *a.p; return *this; }
Bigun & Bigun::operator^=(const Bigun & a) { *p ^= *a.p; return *this; }
Bigun & Bigun::operator&=(const Bigun & a) { *p &= *a.p; return *this; }

Bigun & Bigun::operator<<=(const Bigun & a) { *p <<= *a.p; return *this; }
Bigun & Bigun::operator>>=(const Bigun & a) { *p >>= *a.p; return *this; }

bool Bigun::operator==(const Bigun & a) const { return *p == *a.p; }
bool Bigun::operator<(const Bigun & a) const { return *p < *a.p; }

Bigun Bigun::operator~() const { *p = ~*p; return *this; }
Bigun Bigun::operator-() const { *p = -*p; return *this; }
Bigun & Bigun::operator--() { --*p; return *this; }
Bigun & Bigun::operator++() { ++*p; return *this; }


Bigun Bigun::powmod(Bigun x, Bigun y) const
{
    Bigun r;
    *r.p = this->p->powmod(*x.p, *y.p);
    return r;
}

Bigun Bigun::addmod(Bigun x, Bigun m) const
{
    Bigun r(*this);
    *r.p += *x.p;
    return r %= m;
}

Bigun Bigun::mulmod(Bigun x, Bigun m) const
{
    Bigun r;
    *r.p = this->p->mulmod(*x.p, *m.p);
    return r;
}

Bigun Bigun::submod(Bigun x, Bigun m) const
{
    Bigun r(*this);
    if ( *p < *x.p ) return r += (m - x);
    return r -= x;
}

Bigun Bigun::invmod(Bigun m) const
{
    Bigun r;
    *r.p = this->p->invmod(*m.p);
    return r;
}

bool Bigun::isZero() const
{
    return this->p->isZero();
}

std::vector<uint32_t> Bigun::data() const
{
    return this->p->data();
}

void Bigun::data(const std::vector<uint32_t> & a)
{
    this->p->data(a);
}

} // e3

string e3::Bigun::sth() const
{
    static const char ch[16] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    std::vector<char> stk;

    Bigun x(*this);

    while (!x.isZero())
    {
        auto k = x % 16;
        int q = 0;
        for ( int j = 0; j < 4; j++ ) { int p = 1 << j; q |= (k & Bigun(p)).isZero() ? 0 : p; }
        stk.push_back(ch[q]);
        x >>= 4;
    }

    if ( stk.empty() ) return "0";

    string r;
    for ( auto i = stk.size(); i > 0; ) r += stk[--i];
    return r;
}

e3::Bigun::ull_t e3::Bigun::ull() const
{
    return this->p->ull();
}

void e3::Bigun::BitRef::setbit(Bigun::BitVal a)
{
    if ( !p ) throw "uninitialized bit reference in " + std::string(__func__);
    e3::Bigun b(1); b <<= i;
    *p |= b;
    if ( !a.v ) *p ^= b;
}

e3::Bigun::BitVal e3::Bigun::BitRef::val() const
{
    if ( !p ) throw "uninitialized bit reference in " + std::string(__func__);
	Bigun q(*p);
    return BitVal { !((q >> i) & Bigun(1)).isZero() };
}
// *** end cpp_share/util/def_mpir.inc

// *** begin cpp_share/util/e3util.cpp


using std::string;
using std::vector;

e3::util::ull e3::util::hex2ull(std::string s)
{
    std::stringstream ss (s);
    e3::util::ull x;
    ss >> std::hex >> x;
    return x;
}

std::string e3::util::ull2hex(e3::util::ull x)
{
    std::stringstream ss;
    ss << std::hex << x;
    string s ( ss.str() );
    s = std::string( 2 * sizeof(x) - s.size(), '0').append(s);
    return s;
}

bool e3::util::isHex(const string & s)
{
    for ( auto c : s ) if (!isHex(c)) return false;
    return true;
}

string e3::util::bin2hex(const std::vector<bool> & bin)
{
    std::vector<bool> bits(bin);
    while ( bits.size() % 4 ) bits.push_back(false);
    string hex;
    for ( size_t i = 0; i < bits.size(); i += 4 )
    {
        unsigned dec = 0;
        for (int j = 3; j >= 0; j--)
            dec = (dec << 1) + (unsigned)bits[i + j];
        char c = dec2hex(dec);
        hex = c + hex;
    }
    return hex;
}

std::vector<bool> e3::util::hex2bin(const string & hex, size_t bitsize)
{
    std::vector<bool> bin;
    if (isHex(hex))
    {
        for ( int i = (int)hex.size() - 1; i >= 0; i-- )
        {
            unsigned d = hex2dec(hex[i]);
            for (unsigned j = 0; j < 4; j++)
            {
                bin.push_back(d & 1);
                d >>= 1;
            }

        }
        if ( bitsize ) while ( bin.size() > bitsize ) bin.pop_back();
    }
    return bin;
}

unsigned e3::util::hex2dec(char hex)
{
    if ( hex <= '9' ) return hex - '0';
    return hex - 'a' + 10;
}

char e3::util::hex2dec(char hex, bool * carry)
{
    bool c = false;
    char dec;
    if ( hex >= 'a' )
    {
        c = true;
        dec = hex - 'a' + '0';
    }
    else dec = hex;

    if ( carry ) *carry = c;
    return dec;
}

string e3::util::hex2dec(const string & hexStr, size_t bitsize, bool sign)
{
    string hex (hexStr);
    std::transform( hex.begin(), hex.end(), hex.begin(), ::tolower );
    bool neg = false;
    if ( sign )
    {
        neg = isNegative(hex, bitsize);
        if ( neg ) hex = twosComplement(hex, bitsize);
    }

    string dec = "0";
    for ( auto h : hex )
    {

        dec = multiply(dec, 16);
        unsigned d = hex2dec(h);
        dec = add(dec, d);
    }
    dec = removeLeadingZeros(dec);
    if ( neg ) dec = '-' + dec;
    return dec;
}

char e3::util::dec2hex(unsigned dec)
{
    if ( dec <= 9 ) return (char)(dec + '0');
    return (char)(dec - 10 + 'a');
}

string e3::util::dec2hex(const string & decStr, size_t bitsize)
{
    if (decStr == "0") return zeroExtension("0", bitsize);
    string dec (decStr);
    bool isNegative = false;
    if ( dec[0] == '-' )
    {
        isNegative = true;
        dec = dec.substr(1);
    }

    string hex;
    while ( dec != "0" )
    {
        unsigned m = mod(dec, 16);
        dec = divide(dec, 16);
        hex = dec2hex(m) + hex;
    }

    if ( isNegative ) hex = twosComplement(hex, bitsize);
    else hex = zeroExtension(hex, bitsize);
    return hex;
}

bool e3::util::isNegative(const string & hex, size_t bitsize)
{
    auto maxhexbs = 4 * hex.size();
    if ( bitsize > maxhexbs ) return false;

    auto uppersize = bitsize % 4;
    uppersize = uppersize ? uppersize : 4;
    unsigned signThreshold = 1 << (uppersize - 1);
    unsigned upperhex = hex2dec(hex[hex.size() - (bitsize + 3) / 4]);

    return upperhex >= signThreshold;
}

string e3::util::twosComplement(const string & hex, size_t bitsize)
{
    return increment(negation(hex, bitsize), bitsize);
}

string e3::util::increment(const string & hex, size_t bitsize)
{
    string sum;
    bool c = false;
    int i = (int)hex.size() - 1;
    char s = addHex(hex[i--], '1', &c);
    sum = sum + s;

    for ( ; i >= 0; i-- )
    {
        if ( c ) s = addHex(hex[i], '1', &c);
        else
        {
            s = hex[i];
            c = false;
        }
        sum = s + sum;
    }

    int maxsumbs = 4 * (int)sum.size();
    auto minsumbs = maxsumbs - 3;

    int ibs = (int)bitsize;
    if ( minsumbs > ibs )
        sum = sum.substr(1);
    else if ( minsumbs <= ibs && maxsumbs >= ibs )
        sum[0] = maskHex(sum[0], bitsize);

    return sum;
}

string e3::util::negation(const string & hexStr, size_t bitsize)
{
    string hex (hexStr);
    while ( bitsize > 4 * hex.size() ) hex = '0' + hex;

    string neg;
    for ( auto c : hex )
    {
        neg = neg + negation(c);
    }

    neg[0] = maskHex(neg[0], bitsize);
    return neg;
}

char e3::util::negation(char hex)
{
    switch (hex)
    {
        case '0' : return 'f';
        case '1' : return 'e';
        case '2' : return 'd';
        case '3' : return 'c';
        case '4' : return 'b';
        case '5' : return 'a';
        case '6' : return '9';
        case '7' : return '8';
        case '8' : return '7';
        case '9' : return '6';
        case 'a' : return '5';
        case 'b' : return '4';
        case 'c' : return '3';
        case 'd' : return '2';
        case 'e' : return '1';
        case 'f' : return '0';
    }
    return 0;
}

string e3::util::add(const string & dec, unsigned value)
{
    string r;
    unsigned carry = value;
    for ( int i = (int)dec.size() - 1; i >= 0; i-- )
    {
        unsigned d = dec[i] - '0' + carry;
        carry = d / 10;
        d %= 10;
        r = std::to_string(d) + r;
    }
    if ( carry ) r = std::to_string(carry) + r;
    return r;
}

string e3::util::multiply(const string & dec, unsigned value)
{
    string r;
    unsigned carry = 0;
    for ( int i = (int)dec.size() - 1; i >= 0; i-- )
    {
        unsigned d = (dec[i] - '0') * value + carry;
        carry = d / 10;
        d %= 10;
        r = std::to_string(d) + r;
    }
    if ( carry ) r = std::to_string(carry) + r;
    return r;
}

string e3::util::removeLeadingZeros(const string & s)
{
    string r;
    bool start = false;
    for ( auto c : s )
    {
        if ( start || c != '0' )
        {
            start = true;
            r = r + c;
        }
    }
    if ( r.empty() ) r = "0";
    return r;
}

char e3::util::addHex(char hex1, char hex2, bool * carry)
{
    bool c = false;
    char h1 = hex1 <= '9' ? hex1 - '0' : hex1 - 'a' + 10;
    char h2 = hex2 <= '9' ? hex2 - '0' : hex2 - 'a' + 10;
    char sum = h1 + h2;
    if ( sum <= 9 ) sum += '0';
    else sum += 'a' - 10;

    if ( sum > 'f' )
    {
        c = true;
        sum -= 0x10;
        if ( sum < 'a' ) sum += '0' - 'a' + 10;
    }

    if ( carry ) *carry = c;
    return sum;
}

string e3::util::divide(const string & dec, unsigned div)
{
    string r;
    unsigned i = 0;
    unsigned t = dec[i] - '0';
    while ( t < div ) t = 10 * t + ( ++i < dec.size() ? dec[i] - '0' : 0);
    while ( dec.size() > i )
    {
        r += t / div + '0';
        t = 10 * (t % div) + dec[++i] - '0';
    }
    if ( r.empty() ) r = "0";
    return r;
}

unsigned e3::util::mod(const string & dec, unsigned div)
{
    unsigned r = 0;
    for ( unsigned i = 0; i < dec.size(); i++ )
        r = ( 10 * r + dec[i] - '0' ) % div;
    return r;
}

char e3::util::maskHex(char c, size_t bitsize)
{
    auto uppersize = bitsize % 4;
    uppersize = uppersize ? uppersize : 4;
    auto upperlimit = (1 << uppersize) - 1;
    unsigned upperhex = hex2dec(c);
    upperhex &= upperlimit;
    return dec2hex(upperhex);
}

string e3::util::zeroExtension(const string & hex, size_t bitsize)
{
    int size = int( (bitsize >> 2) - hex.size() + ( bitsize % 4 ? 1 : 0 ) );
    string out = "";
    while (size-- >= 0) out += "0";
    out += hex;
    return out;
}

string e3::util::bin2sbn(const std::vector<bool> & bin)
{
    if ( bin.empty() ) return "0";
    string r;
    for ( auto x : bin ) r += '0' + char(x);
    return r;
}

std::vector<bool> e3::util::sbn2bin(const string & sbin, size_t bitsize)
{
    auto toc = [](char c) -> bool
    {
        if ( c == '0' ) return false;
        if ( c == '1' ) return true;
        throw "Bad character";
    };

    std::vector<bool> bin(bitsize);

    for ( size_t i = 0; i < sbin.size(); i++ ) if ( i < bitsize )
            bin[i] = toc(sbin[i]);

    return bin;
}

vector<string> e3::util::split(const string & s, char c)
{
    vector<string> v;
    string tmp = "";
    for ( auto i : s )
    {
        if ( i == c )
        {
            v.push_back(tmp);
            tmp = "";
        }
        else tmp += i;
    }
    v.push_back(tmp);
    return v;
}

string e3::util::trim(const string & s)
{
    auto posI = s.find_first_not_of(" \t\n\v\r");
    if ( posI == string::npos ) return "";
    auto posF = s.find_last_not_of(" \t\n\v\r");
    return s.substr(posI, posF + 1);
}

namespace
{
string loadConst(string file, int idx)
{
    // add mutex for multithreaded
    static std::map< string, std::vector<string> > mres;

    auto & res = mres[file];

    if ( !res.empty() )
    {
        if ( idx >= (int) res.size() ) throw "Mismatch in file [" + file + "]";
        return res[idx];
    }

    std::ifstream in(file);
    if ( !in ) throw "Cannot open [" + file + "]";
    for (string line; std::getline(in, line); ) res.push_back(line);

    return loadConst(file, idx);
}
} // local

string e3::util::loadConst(string typ, string id)
{
    return ::loadConst( typ + ".const", std::stoi(id.substr(1)) );
}
// *** end cpp_share/util/e3util.cpp

// *** begin cpp_share/util/cophee1.cpp
// Using MPIR library
// Not using CophEE hardware


using std::vector;

namespace e3
{

namespace cophee
{

int Cophee::handler = -1;
int Cophee::handlerArduino = -1;
int Cophee::baudRate = 0;
bool Cophee::inited = false;
bool Cophee::isUsingArduino = false;

Cophee::Cophee(int baudRate, bool isUsingArduino)
{
    Cophee::baudRate = baudRate;
    Cophee::isUsingArduino = isUsingArduino;
}

void Cophee::init() { }

void Cophee::init_arduino()
{
    throw "Cophee: 'init_arduino' shouldn't be called";
}

void Cophee::init_chip()
{
    throw "Cophee: 'init_chip' shouldn't be called";
}

void Cophee::init_port()
{
    throw "Cophee: 'init_port' shouldn't be called";
}

void Cophee::init_port_baud (uint32_t baud)
{
    throw "Cophee: 'init_port_baud' shouldn't be called";
}

uint32_t Cophee::read_serial (uint32_t address) const
{
    throw "Cophee: 'read_serial' shouldn't be called";
}

vector<uint32_t> Cophee::read_serial_n(uint32_t address, size_t size) const
{
    throw "Cophee: 'read_serial_n' shouldn't be called";
}


vector<uint32_t> Cophee::read_serial_1x (uint32_t address) const
{
    throw "Cophee: 'read_serial_1x' shouldn't be called";
}

vector<uint32_t> Cophee::read_serial_2x (uint32_t address) const
{
    throw "Cophee: 'read_serial_2x' shouldn't be called";
}

void Cophee::write_serial (uint32_t address, uint32_t data) const
{
    throw "Cophee: 'write_serial' shouldn't be called";
}

void Cophee::write_serial_n (uint32_t address, const vector<uint32_t> & data, size_t size) const
{
    throw "Cophee: 'write_serial_n' shouldn't be called";
}

void Cophee::write_serial_1x (uint32_t address, const vector<uint32_t> & data) const
{
    throw "Cophee: 'write_serial_1x' shouldn't be called";
}

void Cophee::write_serial_2x (uint32_t address, const vector<uint32_t> & data) const
{
    throw "Cophee: 'write_serial_2x' shouldn't be called";
}

bool Cophee::is_params_set() const
{
    throw "Cophee: 'is_params_set' shouldn't be called";
}

void Cophee::set_params(
    const std::vector<uint32_t> & n,
    const std::vector<uint32_t> & n2,
    const std::vector<uint32_t> & fkf
) const
{
    throw "Cophee: 'set_params' shouldn't be called";
}

void Cophee::set_params(
    const std::vector<uint32_t> & n,
    const std::vector<uint32_t> & n2,
    const std::vector<uint32_t> & rand0,
    const std::vector<uint32_t> & rand1,
    const std::vector<uint32_t> & fkf
) const
{
    throw "Cophee: 'set_params' shouldn't be called";
}

vector<uint32_t> Cophee::mod_mul (const vector<uint32_t> & a, const vector<uint32_t> & b) const
{
    throw "Cophee: 'mod_mul' shouldn't be called";
}

vector<uint32_t> Cophee::mod_exp (const vector<uint32_t> & a, const vector<uint32_t> & b) const
{
    throw "Cophee: 'mod_exp' shouldn't be called";
}

vector<uint32_t> Cophee::mod_inv (const vector<uint32_t> & a, const vector<uint32_t> & b) const
{
    throw "Cophee: 'mod_inv' shouldn't be called";
}

vector<uint32_t> Cophee::gfunc (const vector<uint32_t> & a, const vector<uint32_t> & b) const
{
    throw "Cophee: 'gfunc' shouldn't be called";
}

void Cophee::wait_for_interrupt () const
{
    throw "Cophee: 'wait_for_interrupt' shouldn't be called";
}

} // cophee

} // e3
// *** end cpp_share/util/cophee1.cpp

// *** begin cpp_share/util/base64.cpp


using std::string;

namespace
{
inline void fill2encode(unsigned char * in, unsigned char * out)
{
    out[0] = (in[0] & 0xfc) >> 2;
    out[1] = ((in[0] & 0x03) << 4) + ((in[1] & 0xf0) >> 4);
    out[2] = ((in[1] & 0x0f) << 2) + ((in[2] & 0xc0) >> 6);
    out[3] = in[2] & 0x3f;
}

inline void fill2decode(unsigned char * in, unsigned char * out)
{
    out[0] = (in[0] << 2) + ((in[1] & 0x30) >> 4);
    out[1] = ((in[1] & 0xf) << 4) + ((in[2] & 0x3c) >> 2);
    out[2] = ((in[2] & 0x3) << 6) + in[3];
}

inline const string & b64_table()
{
    static const string table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    return table;
}

} // namespace

string e3::util::base64::encode(unsigned char const * data, unsigned int data_length)
{
    int i = 0;
    int j = 0;
    string encoded_data;
    unsigned char data_block[3];
    unsigned char encoded_block[4];

    while (data_length--)
    {
        data_block[i++] = *(data++);
        if (i == 3)
        {
            fill2encode(data_block, encoded_block);
            for (i = 0; (i < 4) ; i++)
                encoded_data += b64_table()[encoded_block[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            data_block[j] = '\0';

        fill2encode(data_block, encoded_block);

        for (j = 0; (j < i + 1); j++)
            encoded_data += b64_table()[encoded_block[j]];

        while ((i++ < 3))
            encoded_data += '=';

    }

    return encoded_data;

}

string e3::util::base64::decode(string const & data)
{
    int data_length = (int)data.size();
    int i = 0;
    int j = 0;
    int k = 0;
    unsigned char data_block[4], decoded_block[3];
    string decoded_data;

    while (data_length-- && ( data[k] != '=') && (isalnum(data[k]) || (data[k] == '+') || (data[k] == '/')))
    {
        data_block[i++] = data[k];
        k++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                data_block[i] = (unsigned char)b64_table().find(data_block[i]);

            fill2decode (data_block, decoded_block);

            for (i = 0; (i < 3); i++)
                decoded_data += decoded_block[i];
            i = 0;
        }
    }

    if (i != 0)
    {
        for (j = i; j < 4; j++)
            data_block[j] = '\0';

        for (j = 0; j < 4; j++)
            data_block[j] = (unsigned char)b64_table().find(data_block[j]);

        fill2decode (data_block, decoded_block);
        for (j = 0; (j < i - 1); j++) decoded_data += decoded_block[j];
    }

    return decoded_data;
}

// *** end cpp_share/util/base64.cpp

// *** begin cpp_share/util/def_fhew0.cpp


int e3::fhew_impl() { return 0; }

e3::FhewNativeBt::FhewNativeBt(cFhewNativeEvalKey ek): p(new LweCipherText) {}

e3::FhewNativeBt::FhewNativeBt(const std::string & enc, cFhewNativeEvalKey ek)
    : FhewNativeBt(ek)
{
    if ( enc == "0" ) p->b = false;
    else if ( enc == "1" ) p->b = true;
    else throw "Bad init in FhewNativeBt [" + enc + "]";
}

std::string e3::FhewNativeBt::str(cFhewNativeEvalKey ek) const
{
    return p->b ? "1" : "0";
}

e3::FhewNativeBt::FhewNativeBt(const FhewNativeBt & b, cFhewNativeEvalKey ek)
    : FhewNativeBt(ek)
{
    p->b = b.p->b;
}

// *** end cpp_share/util/def_fhew0.cpp

// *** begin cpp_share/util/def_heli0.cpp


int e3::heli_impl() { return 0; }

e3::HeliNativeBt::HeliNativeBt(cHeliNativeEvalKey k): ek(k), ctxt(new HeliCtxt) {}

e3::HeliNativeBt::HeliNativeBt(const std::string & enc, cHeliNativeEvalKey k)
    : HeliNativeBt(k)
{
    if ( enc == "0" ) ctxt->b = false;
    else if ( enc == "1" ) ctxt->b = true;
    else throw "Bad init in HeliNativeBt [" + enc + "]";
}

std::string e3::HeliNativeBt::str(cHeliNativeEvalKey k) const
{
    return ctxt->b ? "1" : "0";
}

e3::HeliNativeBt::HeliNativeBt(const HeliNativeBt & b, cHeliNativeEvalKey k)
    : HeliNativeBt(k)
{
    ctxt->b = b.ctxt->b;
}

// *** end cpp_share/util/def_heli0.cpp

// *** begin cpp_share/util/def_mpir1.cpp


int e3::mpir_impl() { return 1; }

int e3::Bigun::maxbitsz() { return 10000000; } // enough

e3::Bigun::Bigun(string s) : Bigun(0)
{
    p->n = s;
}

std::istream & e3::operator>>(std::istream & is, Bigun & x)
{
    string s;
    is >> s;
    x.p->n = s;
    return is;
}

e3::BigunNative & e3::BigunNative::operator<<=(const BigunNative & a)
{
    unsigned long c = a.n.get_ui();
    while (c--) n = n * 2;
    return *this;
}

e3::BigunNative & e3::BigunNative::operator>>=(const BigunNative & a)
{
    unsigned long c = a.n.get_ui();
    while (c--) n = n / 2;
    return *this;
}


static inline mpz_class mulmod(mpz_class a, mpz_class b, mpz_class m)
{
    return a * b % m;
}

static mpz_class powmod(const mpz_class & x, const mpz_class & p, const mpz_class & m)
{
    if ( p == 0 ) return 1;
    if ( p == 1 ) return mulmod(x, 1, m);
    if ( p == 2 ) return mulmod(x, x, m);

    auto y = powmod(x, p / 2, m);
    auto z = mulmod(y, y, m);

    if ( (p % 2) == 0 ) return z;

    return mulmod(x, z, m);
}

static inline mpz_class invmod(mpz_class a, mpz_class m)
{
    mpz_class rop;
    if ( mpz_invert (rop.get_mpz_t(), a.get_mpz_t(), m.get_mpz_t())  ) return rop;
    return mpz_class(0);
}

string e3::BigunNative::str() const { return n.get_str(); }

e3::BigunNative e3::BigunNative::powmod(BigunNative x, BigunNative m) const
{
    auto z = ::powmod(n, x.n, m.n);
    return BigunNative(z);
}

e3::BigunNative e3::BigunNative::mulmod(BigunNative x, BigunNative m) const
{
    return BigunNative(::mulmod(n, x.n, m.n));
}

e3::BigunNative e3::BigunNative::invmod(BigunNative m) const
{
    return BigunNative(::invmod(n, m.n));
}

bool e3::BigunNative::isZero() const
{
    return 0 == mpz_cmp_ui(n.get_mpz_t(), 0);
}

std::vector<uint32_t> e3::BigunNative::data(size_t nitems) const
{
    throw "Wrong MPIR implementation";
}

void e3::BigunNative::data(const std::vector<uint32_t> & a)
{
    throw "Wrong MPIR implementation";
}
// *** end cpp_share/util/def_mpir1.cpp

// *** begin cpp_share/util/def_pil.cpp



e3::PilQuad::PilQuad(string s)
{
    std::istringstream is(s);
    string sa, sb, sr, sv;
    std::getline(is, sa, ':');
    std::getline(is, sb, ':');
    std::getline(is, sr, ':');
    std::getline(is, sv);

    x.a.n = Bigun(sa);
    x.b.n = Bigun(sb);
    y.a.n = Bigun(sr);
    y.b.n = Bigun(sv);
}

string e3::PilQuad::str() const
{
    std::ostringstream os;
    os << x.a.n << ':' << x.b.n << ':' << y.a.n << ':' << y.b.n;
    return os.str();
}



// *** end cpp_share/util/def_pil.cpp

// *** begin cpp_share/util/def_seal0.cpp


using std::string;
using std::to_string;
using std::vector;

int e3::seal_impl() { return 0; }
std::string e3::SealNativeCiphertext::str() const
{
    if ( p->ct.empty() ) return "";
    string s = to_string(p->ct[0]);
    for ( size_t i = 1; i < p->ct.size(); i++ ) s += "_" + to_string(p->ct[i]);
    return s;
}

namespace e3
{

SealNativeCiphertext::SealNativeCiphertext()
    : p(new SealCiphertext) {}

SealNativeCiphertext::SealNativeCiphertext(const SealNativeCiphertext & a)
    : p(new SealCiphertext)
{
    p->ct = a.p->ct;
}

SealNativeCiphertext::SealNativeCiphertext(const std::string & enc, cSealNativeEvalKey ek)
    : SealNativeCiphertext()
{
    const auto & k = e3seal::toek(ek);
    size_t slots = size_t(k->isBatchEncoder ? k->polyModulusDegree : 1);
    p->ct = vector<uint64_t>(slots, 0);
    auto items = e3::util::split(enc, '_');
    for ( size_t i = 0; i < items.size() && i < slots; i++ ) p->ct[i] = uint64_t( std::stoull(items[i]) );
}

}
// *** end cpp_share/util/def_seal0.cpp

// *** begin cpp_share/util/def_seal_ckks0.cpp


using std::string;
using std::to_string;
using std::vector;

std::string e3::SealCkksNativeCiphertext::str() const
{
    if ( p->ct.empty() ) return "";
    string s = to_string(p->ct[0]);
    for ( size_t i = 1; i < p->ct.size(); i++ ) s += "_" + to_string(p->ct[i]);
    return s;
}

namespace e3
{

SealCkksNativeCiphertext::SealCkksNativeCiphertext()
    : p(new SealCkksCiphertext) {}

SealCkksNativeCiphertext::SealCkksNativeCiphertext(const SealCkksNativeCiphertext & a)
    : p(new SealCkksCiphertext)
{
    p->ct = a.p->ct;
}

SealCkksNativeCiphertext::SealCkksNativeCiphertext(const std::string & enc, cSealCkksNativeEvalKey ek)
    : SealCkksNativeCiphertext()
{
    size_t slots = size_t(e3seal_ckks::toek(ek)->polyModulusDegree);
    p->ct = vector<double>(slots, 0);
    auto items = e3::util::split(enc, '_');
    for ( size_t i = 0; i < items.size() && i < slots; i++ ) p->ct[i] = std::stod(items[i]);
}

}
// *** end cpp_share/util/def_seal_ckks0.cpp

// *** begin cpp_share/util/def_tfhe0.cpp


int e3::tfhe_impl() { return 0; }

e3::TfheNativeBt::TfheNativeBt(cTfheNativeEvalKey ek): p(new LweSample) {}

e3::TfheNativeBt::TfheNativeBt(const std::string & enc, cTfheNativeEvalKey ek) : TfheNativeBt(ek)
{
    if ( enc == "0" ) p->b = false;
    else if ( enc == "1" ) p->b = true;
    else throw "Bad init in TfheNativeBt [" + enc + "]";
}

std::string e3::TfheNativeBt::str(cTfheNativeEvalKey ek) const
{
    return p->b ? "1" : "0";
}

e3::TfheNativeBt::TfheNativeBt(const TfheNativeBt & b, cTfheNativeEvalKey ek) : TfheNativeBt(ek)
{
    p->b = b.p->b;
}

// *** end cpp_share/util/def_tfhe0.cpp

// *** begin cpp_share/util/def_bfv.cpp


using std::string;
using std::to_string;
using std::vector;


std::string e3::BfvProtNativeCiphertext::str() const
{
    throw __FUNCTION__;
}

e3::BfvProtNativeCiphertext::BfvProtNativeCiphertext()
{
    throw __FUNCTION__;
}

e3::BfvProtNativeCiphertext::BfvProtNativeCiphertext(const BfvProtNativeCiphertext &)
{
    throw __FUNCTION__;
}

e3::BfvProtNativeCiphertext::BfvProtNativeCiphertext(const std::string & enc, BfvProtEvalKey * ek)
{
    throw __FUNCTION__;
}

// *** end cpp_share/util/def_bfv.cpp

// *** begin cpp_share/util/def_pali0.cpp


using std::string;

int e3::pali_impl() { return 0; }

e3::PaliBfvNativeCiphertext::PaliBfvNativeCiphertext()
{
    p = std::shared_ptr<e3::PaliBfvCiphertext>(
            new e3::PaliBfvCiphertext {std::vector<int64_t>()});
}

std::string e3::PaliBfvCiphertext::str() const
{
    return e3::util::merge(x, "^");
}

std::string e3::PaliBfvNativeCiphertext::str() const
{
    return !!p ? p->str() : "";
}

e3::PaliBfvNativeCiphertext::PaliBfvNativeCiphertext(const PaliBfvNativeCiphertext & x)
{
    p = x.p;
}

e3::PaliBfvNativeCiphertext::PaliBfvNativeCiphertext(const std::string & enc, const PaliBfvEvalKey * ek)
{
    if ( ek->slots() < 2 )
    {
        e3::pali::vi64 v {int64_t( std::stol(enc) )};
        p = std::shared_ptr<PaliBfvCiphertext>(new e3::PaliBfvCiphertext {v});
        return;
    }

    std::vector<string> sv = e3::util::split(enc, '^');
    e3::pali::vi64 m; for ( auto s : sv ) m.push_back(int64_t(std::stoull(s)));

    p = std::shared_ptr<e3::PaliBfvCiphertext>(new e3::PaliBfvCiphertext {m});
}


e3::PaliBfvNativeCiphertext e3::PaliBfvNativeCiphertext::op_add
(const e3::PaliBfvNativeCiphertext & a, void * pcc) const
{
    e3::PaliBfvNativeCiphertext r;
    r.p = std::shared_ptr<e3::PaliBfvCiphertext>(new PaliBfvCiphertext(*p + *a.p));
    return r;
}


e3::PaliBfvNativeCiphertext e3::PaliBfvNativeCiphertext::op_sub
(const e3::PaliBfvNativeCiphertext & a, void * pcc) const
{
    e3::PaliBfvNativeCiphertext r;
    r.p = std::shared_ptr<e3::PaliBfvCiphertext>(new PaliBfvCiphertext(*p - *a.p));
    return r;
}

e3::PaliBfvNativeCiphertext e3::PaliBfvNativeCiphertext::op_mul
(const e3::PaliBfvNativeCiphertext & a, void * pcc) const
{
    e3::PaliBfvNativeCiphertext r;
    r.p = std::shared_ptr<e3::PaliBfvCiphertext>(new PaliBfvCiphertext(*p ** a.p));
    return r;
}

static e3::PaliBfvCiphertext e3_PaliBfvCiphertext_op
(const e3::PaliBfvCiphertext & a, const e3::PaliBfvCiphertext & b, int op)
{
    e3::PaliBfvCiphertext r;

    int sza = (int)a.x.size();
    int szb = (int)b.x.size();
    int sz = std::max(sza, szb);
    r.x = a.x;
    r.x.resize(sz, 0);

    for ( int i = 0; i < szb; i++ ) /// if ( i < sz2 )
        switch (op)
        {
            case 0: r.x[i] += b.x[i]; break;
            case 1: r.x[i] -= b.x[i]; break;
            case 2: r.x[i] *= b.x[i]; break;
            default: throw "Internal error 88115";
        }

    return r;
}

e3::PaliBfvCiphertext e3::PaliBfvCiphertext::operator+(const e3::PaliBfvCiphertext & a) const
{
    return e3_PaliBfvCiphertext_op(*this, a, 0);
}

e3::PaliBfvCiphertext e3::PaliBfvCiphertext::operator-(const e3::PaliBfvCiphertext & a) const
{
    return e3_PaliBfvCiphertext_op(*this, a, 1);
}

e3::PaliBfvCiphertext e3::PaliBfvCiphertext::operator*(const e3::PaliBfvCiphertext & a) const
{
    return e3_PaliBfvCiphertext_op(*this, a, 2);
}
// *** end cpp_share/util/def_pali0.cpp

// *** begin cpp_share/util/def_pali_sis0.cpp

void e3::pali::load_dummy() {}

// *** end cpp_share/util/def_pali_sis0.cpp

// *** begin cpp_share/util/def_palisade_ckks0.cpp


using std::complex;
using std::string;
using std::vector;

namespace e3
{

// Native constructors

PalisadeCkksNativeCiphertext::PalisadeCkksNativeCiphertext()
{
    p = std::shared_ptr<PalisadeCkksCiphertext>(
            new PalisadeCkksCiphertext { vector<double>() }
        );
}

PalisadeCkksNativeCiphertext::PalisadeCkksNativeCiphertext(const PalisadeCkksNativeCiphertext & ct)
{
    p = ct.p;
}

PalisadeCkksNativeCiphertext::PalisadeCkksNativeCiphertext(const string & strCiphertext, const PalisadeCkksEvalKey * ek)
{
    vector<double> v;
    if ( ek->slots() < 2 )
        v = vector<double> { double( std::stod(strCiphertext) ) };
    else
    {
        vector<string> vs = util::split(strCiphertext, '^');
        for ( auto & s : vs ) v.push_back( std::stod(s) );
    }
    p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(v) );
}

// Native operations

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::add(const PalisadeCkksNativeCiphertext & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p + *a.p) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::add(int a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p + PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::add(double a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p + PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::add(const std::complex<double> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p + PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::add(const std::vector<int> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p + PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::add(const std::vector<double> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p + PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::add(const std::vector<std::complex<double>> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p + PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::sub(const PalisadeCkksNativeCiphertext & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p - *a.p) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::sub(int a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p - PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::sub(double a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p - PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::sub(const std::complex<double> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p - PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::sub(const std::vector<int> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p - PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::sub(const std::vector<double> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p - PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::sub(const std::vector<std::complex<double>> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p - PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::mul(const PalisadeCkksNativeCiphertext & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p ** a.p) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::mul(int a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p * PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::mul(double a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p * PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::mul(const std::complex<double> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    int n = *( reinterpret_cast<int *>(context_ptr) );
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p * PalisadeCkksCiphertext(a, n) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::mul(const std::vector<int> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p * PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::mul(const std::vector<double> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p * PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::mul(const std::vector<std::complex<double>> & a, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext(*p * PalisadeCkksCiphertext(a) ) );
    return r;
}

PalisadeCkksNativeCiphertext PalisadeCkksNativeCiphertext::rot(int s, void * context_ptr) const
{
    PalisadeCkksNativeCiphertext r;
    r.p = std::shared_ptr<PalisadeCkksCiphertext>( new PalisadeCkksCiphertext( p->rotate(s) ) );
    return r;
}

vector<PalisadeCkksNativeCiphertext> PalisadeCkksNativeCiphertext::frot(const std::vector<int> & vs, void * context_ptr) const
{
    vector<PalisadeCkksNativeCiphertext> vr;
    for ( auto s : vs ) vr.push_back( this->rot(s, context_ptr) );
    return vr;
}

// Native functions

string PalisadeCkksNativeCiphertext::str() const
{
    return !!p ? p->str() : "";
}

// Non-native functions

PalisadeCkksCiphertext::PalisadeCkksCiphertext(const vector<int> & a)
{
    x = vector<complex<double>>( a.begin(), a.end() );
}

PalisadeCkksCiphertext::PalisadeCkksCiphertext(const vector<double> & a)
{
    x = vector<complex<double>>( a.begin(), a.end() );
}

PalisadeCkksCiphertext::PalisadeCkksCiphertext(const vector<complex<double>> & a)
{
    x.assign( a.begin(), a.end() );
}

static PalisadeCkksCiphertext e3_PalisadeCkksCiphertext_op(const PalisadeCkksCiphertext & a, const PalisadeCkksCiphertext & b, int op)
{
    PalisadeCkksCiphertext r;
    int sizeA = (int) a.x.size();
    int sizeB = (int) b.x.size();
    int size = std::max(sizeA, sizeB);
    r.x = a.x;
    r.x.resize(size, 0);

    for ( int i = 0; i < size; i++ )
    {
        if ( i < size ) switch (op)
            {
                case 0: r.x[i] += b.x[i]; break;
                case 1: r.x[i] -= b.x[i]; break;
                case 2: r.x[i] *= b.x[i]; break;
                default: throw "Unknown operation (def_palisade_ckks0.cpp)";
            }
    }
    return r;
}

PalisadeCkksCiphertext PalisadeCkksCiphertext::operator+(const PalisadeCkksCiphertext & a) const
{
    return e3_PalisadeCkksCiphertext_op(*this, a, 0);
}

PalisadeCkksCiphertext PalisadeCkksCiphertext::operator-(const PalisadeCkksCiphertext & a) const
{
    return e3_PalisadeCkksCiphertext_op(*this, a, 1);
}

PalisadeCkksCiphertext PalisadeCkksCiphertext::operator*(const PalisadeCkksCiphertext & a) const
{
    return e3_PalisadeCkksCiphertext_op(*this, a, 2);
}

PalisadeCkksCiphertext PalisadeCkksCiphertext::rotate(int s) const
{
    PalisadeCkksCiphertext r;
    auto size = x.size();
    r.x.resize(size);
    for ( size_t i = 0; i < size; i++ ) r.x[i] = x[ (i + s) % size ];
    return r;
}

string PalisadeCkksCiphertext::str() const
{
    return util::merge(x, "^");
}

} // e3
// *** end cpp_share/util/def_palisade_ckks0.cpp

// *** begin cpp_share/util/def_palisade_ckks_util0.cpp

void e3::palisade_ckks::dummy() {}
// *** end cpp_share/util/def_palisade_ckks_util0.cpp

// *** begin cpp_share/keys/anykey.cpp


using std::string;

namespace deco
{
char begin = '[';
char end = ']';
}

namespace e3
{

string AnyKey::decor(const string & s, bool add) const
{
    return prefix(s, add, name.typ);
}

string AnyKey::prefix(const string & a, bool add, string pfx)
{
    auto sz = pfx.size();

    if ( add ) return pfx + deco::begin + a + deco::end;

    const string * ps = &a;
    string fs;

    if ( a.size() > 0 &&  a[0] == '@' )
    {
        fs = util::loadConst(pfx, a);
        ps = &fs;
    }

    const string & s = *ps;

    if ( s.size() <= sz || s.substr(0, sz) != pfx ) return "";

    if ( s.size() < 2 ) return "";
    if ( s[sz] != deco::begin || s[s.size() - 1] != deco::end ) return "";

    return s.substr(sz + 1, s.size() - 2 - sz);
}

} // e3
// *** end cpp_share/keys/anykey.cpp

// *** begin cpp_share/keys/ek_abstract.cpp

using std::string;

void e3::EvalKey::loadOrDie()
{
    if (!load()) throw "Cannot load the key [" + filename() + "]";
}

string e3::EvalKey::encrypt(const string & s, int msz) const
{
    // return empty string for N/A
    return "";
}
// *** end cpp_share/keys/ek_abstract.cpp

// *** begin cpp_share/keys/ek_circ.cpp




using std::string;
using std::vector;

namespace cir_deco
{
char separator = ',';
}

vector<string> e3::CircuitEvalKey::enc2bits(const string & e)
{
    using rt = vector<string>;
    rt r;
    string c = e;

    if (c.size() < 1 ) return rt();

    std::istringstream is(c);
    int i = 0;
    for ( string s; std::getline(is, s, cir_deco::separator); i++ )
        r.push_back( s );

    if ( !i ) return rt();
    return r;
}

string e3::CircuitEvalKey::bits2enc(const vector<string> & v)
{
    string x;
    for ( auto b : v ) x += b + cir_deco::separator;
    return x.substr(0, x.size() - 1);
}

// *** end cpp_share/keys/ek_circ.cpp

// *** begin cpp_share/keys/ek_circ_bdd.cpp


bool e3::CircuitEvalKey_bdd::load() { return true; }

std::string e3::CircuitEvalKey_bdd::filename() const { return EvalKey::filename(); }

// *** end cpp_share/keys/ek_circ_bdd.cpp

// *** begin cpp_share/keys/ek_circ_fhew0.cpp


bool e3::CircuitEvalKey_fhew::load()
{
    std::ifstream in(filename(), std::ios::binary);
    return !!in;
}

std::string e3::CircuitEvalKey_fhew::filename() const
{
    return EvalKey::filenamex("_0");
}

// *** end cpp_share/keys/ek_circ_fhew0.cpp

// *** begin cpp_share/keys/ek_circ_heli0.cpp


bool e3::CircuitEvalKey_heli::load()
{
    std::ifstream in(filename(), std::ios::binary);
    return !!in;
}

std::string e3::CircuitEvalKey_heli::filename() const
{
    return EvalKey::filenamex("_0");
}

// *** end cpp_share/keys/ek_circ_heli0.cpp

// *** begin cpp_share/keys/ek_circ_pil.cpp

void e3::CircuitEvalKey_pilc::dummy() const {}
// *** end cpp_share/keys/ek_circ_pil.cpp

// *** begin cpp_share/keys/ek_circ_plain.cpp


bool e3::CircuitEvalKey_plain::load()
{
    std::ifstream in(filename(), std::ios::binary);
    std::string s;
    in >> s;
    if (!in) return false;
    try { key = std::stoi(s); }
    catch (...) { throw "(l:12) Bad key in " + filename(); }
    return true;
}


// *** end cpp_share/keys/ek_circ_plain.cpp

// *** begin cpp_share/keys/ek_circ_tfhe0.cpp


bool e3::CircuitEvalKey_tfhe::load()
{
    std::ifstream in(filename(), std::ios::binary);
    return !!in;
}

std::string e3::CircuitEvalKey_tfhe::filename() const
{
    return EvalKey::filenamex("_0");
}

// *** end cpp_share/keys/ek_circ_tfhe0.cpp

// *** begin cpp_share/keys/ek_native.cpp


bool e3::NativeEvalKey::load()
{
    std::ifstream in(filename(), std::ios::binary);
    std::string s;
    in >> s;
    if (!in) return false;

    key = std::stoull(s);
    return true;
}
// *** end cpp_share/keys/ek_native.cpp

// *** begin cpp_share/keys/ek_pil.cpp


using std::cout;

const e3::PilEvalValues * e3::PilArith::use = nullptr;

bool e3::PilBaseEvalKey::load(string fname)
{
    std::ifstream in(fname, std::ios::binary);
    if (!in) return false;
    std::string s;

    if ( kv.ver != 22 ) throw "e3::PilBaseEvalKey::load: version";
    in >> s; kv.N = Bigun(s);
    in >> s; kv.S = PilNum(Bigun(s));
    in >> s; kv.A = PilNum(Bigun(s));
    in >> s; kv.B = PilNum(Bigun(s));
    in >> s; kv.C = PilNum(Bigun(s));
    in >> s; kv.D = PilNum(Bigun(s));
    in >> s; kv.E = PilNum(Bigun(s));
    in >> s; kv.F = PilNum(Bigun(s));

    if (!in) return false;
    return true;
}


void e3::PilArith::safe()
{
    if ( use ) return;

    string e = "PilArith is not switched on";
    cout << e << '\n';
    throw e;
}

e3::PilPair e3::operator*(PilPair x, PilPair y)
{
    return {x.a * y.a + x.b * y.b * PilArith::u()->S, x.a * y.b + x.b * y.a};
}


e3::PilQuad e3::operator*(PilQuad a, PilQuad b)
{
    const bool PLAINTEXT = false;
    if (PLAINTEXT ) return {a.x * b.x, a.y * b.y};

    auto u11 = a.x * b.x;
    auto u22 = a.y * b.y;
    auto u12 = a.x * b.y + a.y * b.x;

    const auto & A = PilArith::u()->A;
    const auto & B = PilArith::u()->B;
    const auto & C = PilArith::u()->C;
    const auto & D = PilArith::u()->D;
    const auto & E = PilArith::u()->E;
    const auto & F = PilArith::u()->F;

    auto x = A * u11 + B * u22 + C * u12;
    auto y = D * u11 + E * u22 + F * u12;

    return {x, y};
}

// *** end cpp_share/keys/ek_pil.cpp

// *** begin cpp_share/keys/ek_pail.cpp


using std::cout;

bool e3::PailEvalKey::load(string fname)
{
    std::ifstream in(fname, std::ios::binary);
    if (!in) return false;
    std::string s;

    in >> s;

    setN(Bigun(s));

    int b = 0;
    in >> b;
    if ( b ) setBeta(b);

    if (!in) return false;
    return true;
}

void e3::PailEvalKey::Processor::init(e3::Bigun n)
{
    N = n;
    if (N == Bigun(1))
        throw "N cannot be 1";

    beta = high_bit_posN = high_bit_posN2 = 0;
    N2 = A2 = B2 = Bigun(0);

    Bigun x = N - 1; // if N==0 - Ok;
    // -1 does not matter for N!=2^n
    // but splits in half when N==2^n

    while (x != Bigun(0)) { x >>= 1; high_bit_posN++; }
    high_bit_posN--;

    Xp1 = Bigun(2);
    Xp2 = Bigun(0) - 1;
    Xp2 >>= 1;

    if (N.isZero())
        return; // leave the rest undefined - 0's

    N = n;
    N2 = N * N;

    high_bit_posN2 = 0;
    x = N2;
    while ( x != Bigun(0) ) { x >>= 1; high_bit_posN2++; }
    high_bit_posN2--;

    unsigned nbit = 0;
    while ( (N >> Bigun(++nbit) ) != Bigun(1) );
    A2 = (Bigun(1) << Bigun(nbit));

    Bigun M = N - A2;

    if (M.isZero())
    {
        if (high_bit_posN > 1)
            setB2Beta(high_bit_posN - 1);
        else
        {
            // leave beta and B2 undefined - 0's
        }
    }
    else
    {
        nbit = 0;
        while ((M >> ++nbit ) > Bigun(1) );

        setB2Beta(nbit);

        if (B2 * 2 <= A2) {}
        else
            setB2Beta(nbit - 1);
    }

    Bigun tpmax = (Bigun(1) << high_bit_posN) - 1;
    Xp1 = N + 1;
    Xp2 = N * (tpmax + 1);
}

void e3::PailEvalKey::Processor::setB2Beta(int b)
{
    if (beta && b > beta)
    {
        std::cout << "Current beta " << beta << "; new value " << b << '\n';
        throw "Setting beta above predefined is forbidden; increase N";
    }

    if (N.isZero() && b > high_bit_posN / 2 )
    {
        std::cout << "Try setting beta " << b << ", but allowed value is " << (high_bit_posN / 2) << '\n';
        throw "Setting beta too high";
    }

    beta = b;
    B2 = (Bigun(1) << b);
}

string e3::PailEvalKey::Processor::show() const
{
    std::ostringstream os;
    os << "N=" << N << " N2=" << N2;
    os << " M=" << (N - A2) << " A2=" << A2;
    os << " B2=" << B2 << " beta=" << beta;
    os << " high_bit_posN=" << high_bit_posN;
    return os.str();
}

// *** end cpp_share/keys/ek_pail.cpp

// *** begin cpp_share/keys/ek_seal0.cpp


using std::stoi;
using std::stoull;
using std::string;

namespace e3
{

bool SealBaseEvalKey::load(string fname)
{
    std::ifstream in(fname, std::ios::binary);

    if ( !in ) throw "Cannot open " + fname;

    try
    {
        string s;
        e3seal::SealEvalKey * ekey = new e3seal::SealEvalKey;
        e3seal::SealEvalKey & evalkey = *ekey;
        getline(in, s);
        evalkey.polyModulusDegree = uint64_t( stoull(s) );
        getline(in, s);
        evalkey.plaintextModulus  = uint64_t( stoull(s) );
        getline(in, s);
        evalkey.isBatchEncoder    = bool( stoi(s) );
        key = ekey;
    }
    catch (...) { throw "Bad data in " + fname; }

    return !!in;
}

string SealBaseEvalKey::rawEncrypt(const string & s, int) const
{
    string strout;
    auto evalkey = e3seal::toek(key);
    auto & isBatchEncoder = evalkey->isBatchEncoder;
    if ( isBatchEncoder )
    {
        auto v = e3::util::split(s, '_');
        auto & slots = evalkey->polyModulusDegree;
        v.resize(size_t(slots), v.back());
        strout = e3::util::merge(v, "_");
    }
    else strout = s;
    return strout;
}

size_t SealBaseEvalKey::slots()
{
    const auto & k = e3seal::toek(key);
    return size_t(k->isBatchEncoder ? k->polyModulusDegree : 1);
}

} // e3
// *** end cpp_share/keys/ek_seal0.cpp

// *** begin cpp_share/keys/ek_seal_ckks0.cpp


using std::stoull;
using std::string;

namespace e3
{

bool SealCkksBaseEvalKey::load(string fname)
{
    std::ifstream in(fname, std::ios::binary);
    string s;
    static e3seal_ckks::SealCkksEvalKey evalkey;
    getline(in, s);
    evalkey.polyModulusDegree = uint64_t( stoull(s) );

    getline(in, s);
    evalkey.scale = uint64_t ( stoull(s) );

    key = &evalkey;
    return !!in;
}

string SealCkksBaseEvalKey::rawEncrypt(const string & s, int msz) const
{
    string tmp = s;
    std::replace(tmp.begin(), tmp.end(), 'o', '.');
    return tmp;
}

size_t SealCkksBaseEvalKey::slots()
{
    const auto & k = e3seal_ckks::toek(key);
    return (size_t)k->polyModulusDegree;
}

} // e3
// *** end cpp_share/keys/ek_seal_ckks0.cpp

// *** begin cpp_share/keys/ek_bfv_prot.cpp


using std::stoi;
using std::stoull;
using std::string;

namespace e3
{

bool BfvProtEvalKey::load(string fname)
{
    std::cout << "ek_bfv_prot.cpp BfvProtEvalKey::load - NI\n";
    return false;
}

string BfvProtEvalKey::rawEncrypt(const string & s, int) const
{
    // FIXME refactor with SEAL
    string strout;
    if ( nslots > 1 )
    {
        auto v = e3::util::split(s, '_');
        auto & nnslots = polyModulusDegree;
        v.resize(size_t(nnslots), v.back());
        strout = e3::util::merge(v, "_");
    }
    else strout = s;
    return strout;

}

size_t BfvProtEvalKey::slots()
{
    return nslots;
}

} // e3
// *** end cpp_share/keys/ek_bfv_prot.cpp

// *** begin cpp_share/keys/ek_pali0.cpp



using std::string;

namespace e3
{

struct PaliBfvQuery0 : e3::PaliBfvQuery
{
    virtual void print() const;
} paliBfvQuery0;

bool PaliBfvEvalKey::load(string fname)
{
    std::ifstream in(filename(), std::ios::binary);

    if ( !in ) return false;
    in >> polyDegree >> mulDepth >> useSlots >> maxDepth >> p_n;

    if ( !in ) return false;

    key.pk = &useSlots;

    pali::load_dummy();

    return true;
}

string PaliBfvEvalKey::rawEncrypt(const string & s, int msz) const
{
    e3::pali::vi64 v;
    if ( useSlots > 1 )
    {
        auto vs = e3::util::split(s, '_');
        for ( auto e : vs ) v.push_back( (int64_t) stoll(e) );
        v.resize( slots(), v.back() );
    }
    else v.push_back( (int64_t) stoll(s) );

    PaliBfvCiphertext ct {v};

    return ct.str();
}

size_t PaliBfvEvalKey::slots() const
{
    return useSlots;
}

e3::PaliBfvQuery * PaliBfvEvalKey::query() const
{
    return &e3::paliBfvQuery0;
}

void PaliBfvQuery0::print() const
{
    std::cout << "No parameters for pali0\n";
}

} // e3
// *** end cpp_share/keys/ek_pali0.cpp

// *** begin cpp_share/keys/ek_palisade_ckks0.cpp


using namespace std;

namespace e3
{

bool PalisadeCkksEvalKey::load(string fname)
{
    ifstream fin( fname, std::ios::binary );
    if ( !fin ) return false;
    fin >> multDepth >> scaleFactorBits >> batchSize >> securityLevel >> ringDimension;
    if ( !fin ) return false;
    palisade_ckks::dummy();
    key.cc = &ringDimension;
    return true;
}

string PalisadeCkksEvalKey::rawEncrypt(const string & strPlaintext, int msz) const
{
    vector<double> v;
    auto vs = util::split(strPlaintext, '_');
    for ( auto e : vs ) v.push_back( (double) stod(e) );
    v.resize( slots(), 0 );
    PalisadeCkksCiphertext ciphertext {v};
    return ciphertext.str();
}

size_t PalisadeCkksEvalKey::slots() const
{
    // return batchSize;
    return ringDimension >> 1;
}

} // e3
// *** end cpp_share/keys/ek_palisade_ckks0.cpp

#include <iostream>
#include <fstream>
#include <gmp.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "cofhe.hpp"

using namespace e3;
using namespace e3::cofhe;
using namespace std;

uint8_t * convert(uint64_t din);
uint8_t * convert(const vector<uint64_t> & din);
vector<vector<uint64_t>> load(const string & filename);
uint64_t modInverse(uint64_t number, uint64_t modulus);

int main()
{
    map<string,DataType> params;
    params["portname"] = "/dev/ttyUSB1";
    params["baudrate"] = 1000000;
    string filename = "_ntt_1_i.log";

    Cofhe cofhe(UART, params);
    auto data = load(filename);

    cout << " " << data.size() << " x [ " << flush;
    for ( size_t i=0; i<data.size(); i++ ) cout << data[i].size() << " ";
    cout << "]\n";

    auto ninv    = convert ( modInverse( data[0][0], data[0][1] ) );
    auto n       = uint32_t( data[0][0] );
    auto modulus = convert ( data[0][1] );
    auto poly    = convert ( data[1]    );
    auto twiddle = convert ( data[2]    );
    auto ntt     = convert ( data[4]    );
    auto logq    = uint32_t(128);
    Register regTwiddle = R2;

    cout << "configuring .. " << flush;
    cofhe.configure(n, logq, modulus, ninv, twiddle, regTwiddle);
    cout << "ok\n";

    cout << "loading polynomial .. " << flush;
    cofhe.setRegister(poly, R0);
    // cofhe.setRegister(ntt, R0);
    cout << "ok\n";

    // auto regIntt = cofhe.intt(R0, R1);

    cout << "ntt .. " << flush;
    // auto regOut = cofhe.ntt(regIntt, R3);
    auto regNtt = cofhe.ntt(R0, R3);
    cout << "ok\n";

    cout << "cmul .. " << flush;
    auto regCmul = ( regNtt == R0 ? R3 : R0 );
    cofhe.cmul(regCmul, regNtt);
    sleep(2);
    cout << "ok\n";

    cout << "intt .. " << flush;
    auto regTmp = regNtt;
    auto regIntt = cofhe.intt(regCmul, regTmp);
    sleep(2);
    cout << "ok\n";

    cout << "getting result .. " << flush;
    // auto nttOut = cofhe.getRegister(regOut);
    auto nttOut = cofhe.getRegister(regIntt);
    cout << "ok\n";

    size_t count = 0;
    size_t limit = 16;
    for ( size_t i=0; i < n * bit2byte(logq); i++ )
    {
        if ( i<limit )
            cout << hex << uint32_t(nttOut[i]) << ":" << uint32_t(poly[i]) << '\n';
        if ( nttOut[i] != poly[i] )
        {
            count++;
        }
    }
    cout << dec << "count: " << count << '\n';
}

uint8_t * convert(uint64_t din)
{
    auto s    = sizeof(uint64_t);
    auto size = s << 1;
    auto dout = (uint8_t *) malloc(size);
    size_t i;
    for ( i=0; i<s   ; i++ ) dout[i] = ( din >> byte2bit(i) ) & 0xff;
    for (    ; i<size; i++ ) dout[i] = 0;
    return dout;
}

uint8_t * convert(const vector<uint64_t> & din)
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

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <unistd.h>

#include "cofhe_const.hpp"
#include "cofhe.hpp"

using namespace e3;
using namespace e3::cofhe;
using namespace std;

template <class T> int error(const T & e) { cout << "error: " << e << '\n'; return 1; }
void testNtt(Cofhe &, uint32_t, uint32_t);

int main()
try
{
    map<string,DataType> params;
    params["portname"] = "/dev/ttyUSB1";
    params["baudrate"] = 1000000;
    uint32_t n = 4096;
    uint32_t logq = 128; // bits

    Cofhe cofhe(UART, params);
    testNtt(cofhe, n, logq);
}
catch (const char   * e) { return error(e); }
catch (const string & e) { return error(e); }

void testNtt(Cofhe & cofhe, uint32_t n, uint32_t logq)
{
    // auto poly = generatePolynomial(n, logq);
    uint32_t logqBytes = bit2byte(logq);
    auto nBytes = n * logqBytes;
    uint8_t in[nBytes];
    uint8_t mem[8][nBytes];
    uint32_t data_in, mem_addr, mem_idx, mem_value;
    uint8_t * data_in_temp;
    FILE *fr;

    data_in_temp = (uint8_t *) &data_in;

    fr = fopen("../../../c/uart_in_poly_mul.txt","r");

    if (fr == NULL)
    {
        printf("File open Error!");
        exit(1);
    }
    printf("ok\n");

    while(fscanf(fr, "%08x\n", &data_in) == 1)
    {
        if (data_in == cofhe::CMD_READ) // data read
        {
            if(fscanf(fr, "%08x\n", &data_in) == 1)
            {
                data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
                data_in_temp[2] = data_in_temp[1]^data_in_temp[2];
                data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
                data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
                data_in_temp[3] = data_in_temp[0]^data_in_temp[3];
                data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
            }
        }
        else if (data_in == cofhe::CMD_WRITE) // Data Write
        {
            if (fscanf(fr, "%x\n", &data_in) == 1)
            {
                data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
                data_in_temp[2] = data_in_temp[1]^data_in_temp[2];
                data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
                data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
                data_in_temp[3] = data_in_temp[0]^data_in_temp[3];
                data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
                if (data_in > cofhe::GPCFG_BASE)
                {
         	        if (fscanf(fr, "%x\n", &data_in) == 1)
                    {
                        data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
                        data_in_temp[2] = data_in_temp[1]^data_in_temp[2];
                        data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
                        data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
                        data_in_temp[3] = data_in_temp[0]^data_in_temp[3];
                        data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
                    }
                }
                else
                {
                    mem_idx  = ( data_in >> byte2bit(3) ) - 0x20;
                    mem_addr = data_in % power2( byte2bit(3) );
                    // printf("Writing Memory %d -> Address -> %x\n", mem_idx, mem_addr);
                    if (fscanf(fr, "%x\n", &data_in) == 1)
                    {
                        data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
                 		data_in_temp[2] = data_in_temp[1]^data_in_temp[2];
                 		data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
                 		data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
                 		data_in_temp[3] = data_in_temp[0]^data_in_temp[3];
                 		data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
                   		mem_value = data_in;
                        mem[mem_idx][mem_addr  ] =   mem_value;
                        mem[mem_idx][mem_addr+1] = ( mem_value >> byte2bit(1) ) % power2( byte2bit(1) );
                   		mem[mem_idx][mem_addr+2] = ( mem_value >> byte2bit(2) ) % power2( byte2bit(1) );
                   		mem[mem_idx][mem_addr+3] = ( mem_value >> byte2bit(3) ) % power2( byte2bit(1) );
                    }
                }
            }
        }
    }

    for( uint32_t i=0; i<n; i++ )
        for( uint32_t j=0; j<logqBytes; j++ )
            in[i*logqBytes+j] = 0x55 + i*logqBytes + j;

    uint8_t * out;
    uint32_t modulus[] = { 0x3001, 0, 0, 0 };
    uint32_t nInv[]    = { 0x2ffe, 0, 0, 0 };
    auto poly1   = (uint8_t *) &mem[1];
    // auto poly2   = (uint8_t *) &mem[2];
    auto twiddle = (uint8_t *) &mem[3];
    Register regTwiddle = R2;
    cofhe.configure(n, logq, (uint8_t *) &modulus, (uint8_t *) &nInv, twiddle, regTwiddle);
    // out = cofhe.polymul(poly1, poly2, R0, R1, R3);
    cofhe.setRegister(poly1, R0);
    auto regNtt = cofhe.ntt(R0, R3);
    auto regCmul = ( regNtt == R0 ? R3 : R0 );
    // cout << regOut << " <- NTT\n";
    cofhe.cmul(regCmul, regNtt);
    auto regTmp = regNtt;
    auto regOut = cofhe.intt(regCmul, regTmp);
    sleep(2);
    out = cofhe.getRegister(regOut);
    // out = cofhe.getRegister(R0);

    size_t count = 0;
    for ( uint32_t i=0; i < n * bit2byte(logq); i++ )
        if ( poly1[i] != out[i] )
        {
            cout << hex << i << " expected " << uint32_t(poly1[i]) << " got " << uint32_t(out[i]) << '\n';
            count++;
        }
    cout << "Count: " << dec << count << '\n';

    // for( uint32_t i=0; i<n; i++ )
    //     printf("out[%d] =%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",i,out[i*16],out[i*16+1],out[i*16+2],out[i*16+3],out[i*16+4],out[i*16+5],out[i*16+6],out[i*16+7],out[i*16+8],out[i*16+9],out[i*16+10],out[i*16+11],out[i*16+12],out[i*16+13],out[i*16+14],out[i*16+15]);
}

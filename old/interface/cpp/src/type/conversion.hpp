#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "number.h"
#include "ringint.hpp"

namespace conversion
{

uint8_t   charhextouint8(char c);
std::string fillhexstr(const std::string & s, int nChars);
bool isHexPrefix(const std::string & s);
std::string removeHexPrefix(const std::string & s);
uint8_t   strhexbytetouint8(const std::string & s);
uint8_t * strhextouint8(const std::string & s);
uint8_t * strhextouint8(const std::string & s, int nBytes);
uint8_t * strhextouint8(const std::vector<std::string> & v);
uint8_t * strhextouint8(const std::vector<std::string> & v, int nBytes);
std::string strRepeat(char c, int n);
Number toNumber(const uint8_t * n, int nBytes);
std::vector<Number> toNumber(const uint8_t * n, int nItems, int nBytes);
Ringint toRingint(const uint8_t * n, int nBytes);
std::vector<Ringint> toRingint(const uint8_t * n, int nItems, int nBytes);
uint8_t * toUint8(const Number & n, int nBytes);
uint8_t * toUint8(const Ringint & n, int nBytes);
uint8_t * toUint8(const std::vector<Number> & v, int nBytes);
uint8_t * toUint8(const std::vector<Ringint> & v, int nBytes);
uint8_t * uint64touint8(uint64_t din);
uint8_t * uint64touint8(const std::vector<uint64_t> & din);
char uint8tocharhex(uint8_t n);
std::string uint8tostrhex(const uint8_t * n, int nBytes);
std::vector<std::string> uint8tostrhex(const uint8_t * n, int nItems, int nBytes);
uint64_t  uint8touint64(const uint8_t * n);
// uint8_t * header_uint64touint8(uint64_t din)
// {
//     auto s    = sizeof(uint64_t);
//     auto size = s << 1;
//     auto dout = (uint8_t *) malloc(size);
//     size_t i;
//     for ( i=0; i<s   ; i++ ) dout[i] = ( din >> e3::cofhe::byte2bit(i) ) & 0xff;
//     for (    ; i<size; i++ ) dout[i] = 0;
//     return dout;
// }

} // conversion

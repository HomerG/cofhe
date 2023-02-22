#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "number.h"
#include "ringint.h"

namespace io
{

std::vector<std::vector<uint64_t>> load(const std::string & filename);
void print(const uint8_t * din, size_t size, size_t divider=0);
void printPair(const Number & n, const uint8_t * u, size_t size);
void printPair(const std::string & n, const uint8_t * u, size_t size);
void printPair(const std::vector<Number> & vn, const uint8_t * u, size_t size);
void printPair(const std::vector<std::string> & vn, const uint8_t * u, size_t size);
void printShort(const std::vector<Ringint> &);
std::vector<std::string> read(const std::string & filename);

} // io

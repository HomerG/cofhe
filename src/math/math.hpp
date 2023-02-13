#pragma once

#include <cstdint>
#include <vector>
#include "number.h"
#include "ringint.hpp"

namespace math
{

struct MathParams
{
    Number k;
    int n;
    Number q;
    uint32_t barrett_2k;  // Barrett's constant
    Number barrett_md; // Barrett's constant
    Ringint in;
    Ringint rou1n;
    Ringint rou2n;
    Ringint irou2n;
    std::vector<Ringint> twiddle;
    std::vector<Ringint> psi;
    std::vector<Ringint> ipsi;

    void init();
    void print();
    MathParams(){}
    MathParams(int n) : MathParams(n, 1) {}
    MathParams(int n, const Number & k) { this->n = n; this->k = k; init(); }
};

size_t bitReverse(size_t index, size_t bit_length);
// std::vector<Ringint> calcTwiddle(
std::vector<uint64_t> calcTwiddle(uint64_t nth_rou, uint64_t modulus, uint32_t n);
bool check1NthRoot(int n, const Number & m, const Number & om);
bool check2NthRoot(int n, const Number & m, const Number & psi);
std::vector<Ringint> computePolynomial(const std::vector<Ringint> & p, const Ringint & row2n);
std::vector<Ringint> computePowers(int n, const Ringint & base);
Number find1NthRoot(int n, const Number & m, bool throws);
Number find2NthRoot(int n, const Number & m, bool throws);
Ringint find1NthRootOfUnity(int n);
Ringint find2NthRootOfUnity(int n);
bool is1NthRootOfUnity(const Ringint & z, int n);
bool is2NthRootOfUnity(const Ringint & z, int n);
bool isRootOfUnity(const Ringint & z, int n);
std::vector<Ringint> hadamard(const std::vector<Ringint> & p1, const std::vector<Ringint> & p2);
uint64_t modInverse(uint64_t number, uint64_t modulus);
std::vector<Number> primitiveRoots(const Number & modulus, int cmul, int nRoots);
std::vector<uint64_t> primitiveRoots(uint64_t modulus, uint64_t ninv, uint64_t nroots);
std::vector<Ringint> randomRingintPolynomial(int n);
std::vector<uint64_t> reverseTwiddle(const std::vector<uint64_t> & twiddle);

template <class T> int flog2(T k) // floor(log2)
{
    if (k <= 0) throw "Cannot calculate the logarithm of non-positive numbers";
    auto l = 0;
    while (k >>= 1) l++;
    return l;
}

template <class T> int clog2(T k) // ceil(log2)
{
    auto l = flog2(k);
    return k == 1<<l ? l : l+1;
}

} // math

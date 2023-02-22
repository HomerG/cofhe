#pragma once

#include <vector>
#include <string>

#include "number.h"

struct Poly
{
    int n, d;
    Number m;
    std::vector<Number> vc;
    Poly() {}
    Poly(int an, Number am): n(an), m(am), vc(n, Number(0)) {}

    std::string str() const;
};

namespace poly
{
std::vector<Poly> load(std::string filename);

Poly reduceModHalf(const Poly & a);
Poly reduceModN(const Poly & a, int n);

// ntt
// intt

} // poly

Poly operator-(const Poly & a, const Poly & b);
Poly operator+(const Poly & a, const Poly & b);
Poly operator&(const Poly & a, const Poly & b); // direct multiplication
inline Poly operator*(const Poly & a, const Poly & b) // mod x^n+1
{
    return poly::reduceModHalf(a & b);
}

Poly operator%(const Poly & a, const Poly & b);
inline Poly & operator*=(Poly & a, const Poly & b) { return a = a * b; }
inline Poly & operator&=(Poly & a, const Poly & b) { return a = a & b; }

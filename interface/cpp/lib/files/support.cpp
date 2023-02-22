#include "support.h"
#include <iostream>

namespace support
{

Polynomial<Ringint> generatePolynomial(int n, int logq)
{
    auto mod = Number::randomPrime(logq);
    Ringint::setModulus(mod);
    Polynomial<Ringint> poly;
    while (n--) poly[n] = Ringint::random();
    return poly;
}

} // support

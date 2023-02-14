#pragma once

#include "number.h"
#include "polynomial.hpp"
#include "ringint.hpp"

namespace support
{

Polynomial<Ringint> generatePolynomial(int n, int logq);

} // support

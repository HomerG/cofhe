#pragma once

#include "number.h"
#include "polynomial.h"
#include "ringint.h"

namespace support
{

Polynomial<Ringint> generatePolynomial(int n, int logq);

} // support

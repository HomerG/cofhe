// 109-bit q

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <gmp.h>

#include "math.hpp"
#include "number.h"
#include "ringint.hpp"
#include "support.hpp"

using namespace math;
using namespace std;
using namespace support;

template <class T> int error(const T & e) { cout << "error: " << e << '\n'; return 1; }

int main()
try
{
    // mpz_t z;
    // mpz_init_set_str(z, "188981757975021318420037633", 10);
    // gmp_printf("q = %z\n", z);
    // cout << "Prime? ? " << mpz_probab_prime_p(z, 50) << '\n';

    uint32_t logn = 13;
    uint32_t n = 1 << logn;
    Number q = "4375320215957354709071970336841400321";
    // cout << "q: " << q << " " << q.isPrime() << '\n';
    Number k = (q-1) / (2*n);
    MathParams mp(n, k);
    mp.print();
}
catch (const char   * e) { return error(e); }
catch (const string & e) { return error(e); }

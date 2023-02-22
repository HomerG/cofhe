#include <iostream>
#include <vector>
#include "io.hpp"
#include "vector_polynomial.hpp"
#include "ringint.hpp"

using namespace io;
using namespace polynomial;
using namespace std;
using Type = Ringint;

int main()
try
{
    Type::setModulus(77);

    vector<Type> poly = { 9, 0, 5, 7, 1, 3, 1 };
    vector<Type> mod  = { 1, 0, 1 };

    cout << "Polynomial: "; print(poly);
    cout << "Modulus   : "; print(mod);

    auto reducedPoly = reduce(poly, mod);
    cout << "Reduced   : "; print(reducedPoly);
}
catch (const char * e) { cout << e << '\n'; }

#include <iostream>
#include <vector>
#include "io.hpp"
#include "polynomial.hpp"
#include "ringint.hpp"

using namespace io;
using namespace polynomial;
using namespace std;
using Type = Ringint;

int main()
try
{
    Type::setModulus(77);

    Polynomial poly( vector<Type>{ 9, 0, 5, 7, 1, 3, 1 } );
    Polynomial mod ( vector<Type>{ 1, 0, 1, 0 } );
    Polynomial div ( vector<Type>{ 1, 2, 1, 2, 1, 2, 1 } );
    Type s = 3;

    cout << "Polynomial: { " << poly << " }\n";
    cout << "Modulus   : { " << mod  << " }\n";
    cout << "Division  : { " << div  << " }\n";

    auto & a = poly;
    auto & b = mod;
    cout << "\nElement-wise operations\n";
    cout << "ADD: { " << (a+b) << " }\n";
    try { cout << "DIV: { " << (a|div) << " }\n"; }
    catch (const char * e) { cout << e << " }\n"; }
    try { cout << "INV: { " << (~div) << " }\n"; }
    catch (const char * e) { cout << e << " }\n"; }
    cout << "MUL: { " << (a&b) << " }\n";
    cout << "SUB: { " << (a-b) << " }\n";

    cout << "\nPolynomial operations\n";
    try { cout << "DIV: { " << (a/b) << " }\n"; }
    catch (const char * e) { cout << e << " }\n"; }
    try { cout << "MUL: { " << (a*b) << " }\n"; }
    catch (const char * e) { cout << e << " }\n"; }
    try { cout << "RED: { " << (a%b) << " }\n"; }
    catch (const char * e) { cout << e << " }\n"; }

    cout << "\nPolynomial-scalar operations\n";
    cout << "ADD: { " << (a+s) << " }\n";
    cout << "DIV: { " << (a/s) << " }\n";
    cout << "MOD: { " << (a%s) << " }\n";
    cout << "MUL: { " << (a*s) << " }\n";
    cout << "SUB: { " << (a-s) << " }\n";

    cout << "\nPolynomial comparisons\n";
    cout << "EQ: { " << boolalpha << (a==b) << " }\n";
    cout << "GE: { " << boolalpha << (a>=b) << " }\n";
    cout << "GT: { " << boolalpha << (a> b) << " }\n";
    cout << "LE: { " << boolalpha << (a<=b) << " }\n";
    cout << "LT: { " << boolalpha << (a< b) << " }\n";
    cout << "NE: { " << boolalpha << (a!=b) << " }\n";

    cout << "\nCoefficient indexing\n";
    cout << "{ " << a << " }\n";
    cout << "ACCESS: " << ( a[2] ) << '\n';
    cout << "{ " << a << " }\n";
    a[2] = 13;
    cout << "MODIFY: " << ( a[2] ) << '\n';
    cout << "{ " << a << " }\n";
    cout << "ACCESS: " << ( a[9] ) << '\n';
    cout << "{ " << a << " }\n";
    a[15] = 15;
    cout << "MODIFY: " << ( a[15] ) << '\n';
    cout << "{ " << a << " }\n";

    cout << "\nFunctions\n";
    cout << "ORDER: " << a.order() << '\n';
    cout << "SIZE : " << a.size() << '\n';

    cout << '\n';
}
catch (const char * e) { cout << e << '\n'; }

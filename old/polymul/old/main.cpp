#include <iostream>
#include <vector>
#include <iomanip>

#include "chron.h"
#include "coutlog.h"

#include "poly.h"

std::string getMaxNumber()
{
    const int cycles = 1000;
    Number n(1);
    for ( int i = 0; i < cycles; i++ )
    {
        auto p = n;
        n *= Number(4);
        if ( n % 3 != Number(1) ) return "0x" + p.sth();
    }

    return "big";
}

int main(int ac, const char * av[])
try
{
    cout << "Max Number : " << getMaxNumber() << '\n';

    std::vector<Poly> vp = poly::load("poly.in");

    if ( vp.size() < 4 ) never;

    for ( auto p : vp ) cout << p.str() << '\n';
    cout << '\n';

    auto q1 = vp[0], q2 = q1;
    auto & p = vp[0];

    p &= p; cout << p.str() << '\n';
    p = poly::reduceModHalf(p); cout << p.str() << '\n';
    q1 *= q1; cout << q1.str() << '\n';
    q2 &= q2;
    q2 = poly::reduceModN(q2, 4); cout << q2.str() << '\n';

    cout << '\n';
}

catch (int e)
{
    cout << "error (int): " << e << "\n";
    return 1;
}
catch (string e)
{
    cout << "error (str): " << e << "\n";
    return 1;
}
catch (const char * e)
{
    cout << "error (cc): " << e << "\n";
    return 1;
}
catch (...)
{
    cout << "Unknown exception\n";
    return 1;
}


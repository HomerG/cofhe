#include <iostream>
#include <vector>
#include "conversion.hpp"
#include "number.h"
#include "ringint.hpp"

using namespace conversion;
using namespace std;

const int HEX_BASE = 16;

template <class T>
void print(const vector<T> & v)
{
    cout << "{ ";
    for (auto & e : v) cout << e << ' ';
    cout << "}\n";
}

int main()
{
    auto nBytes = 128 >> 3;
    Ringint::setModulus(7);
    Number n = 99;
    Ringint r = 9;
    cout << "n: " << n << '\n';
    cout << "r: " << r << '\n';
    auto un = toUint8(n, nBytes);
    auto ur = toUint8(r, nBytes);
    n = toNumber(un, nBytes);
    r = toRingint(ur, nBytes);
    cout << "n: " << n << '\n';
    cout << "r: " << r << '\n';

    vector<Number> vn;
    vector<Ringint> vr;
    int nItems = 12;
    for (int i=0; i<nItems; i++)
    {
        vn.push_back(  Number(i) );
        vr.push_back( Ringint(i) );
    }
    cout << "vn: "; print(vn);
    cout << "vr: "; print(vr);
    auto uvn = toUint8(vn, nBytes);
    auto uvr = toUint8(vr, nBytes);
    vn = toNumber(uvn, nItems, nBytes);
    vr = toRingint(uvr, nItems, nBytes);
    cout << "vn: "; print(vn);
    cout << "vr: "; print(vr);
}

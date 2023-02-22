#include <iostream>
#include <numeric>
#include <string>

template <class T, class U>
void checkResult(const std::string & operatorName, const T & result, const U & expectedResult)
{
    bool isEqual = result == T(expectedResult);
    std::cout << operatorName << " .. " <<  ( isEqual ? "PASSED" : "FAILED" );
    if ( !isEqual ) std::cout << "\tEXPECTED: " << expectedResult << " GOT: " << result;
    std::cout << '\n';
    assert(isEqual);
}

template <class T>
T inv(const T & a, const T & mod)
{
    if (mod == 1) return 0;
    T r = 1;
    for (T i=0; i<mod; i++, r++)
        if ( (r * a) % mod == 1 ) return r;
    throw "Inverse not found";
}

template <class T>
T lcm(const T & a, const T & b)
{
    return a && b ? (a * b) / std::gcd(a, b) : T(0);
}

template <class T>
T pow(const T & base, const T & exp, const T & mod)
{
    if (mod == 1) return 0;
    T r = 1;
    for (auto e = exp; e; e--) r = (r * base) % mod;
    return r;
}

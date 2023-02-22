
#include <fstream>
#include <sstream>

#include "poly.h"

using std::string;

std::vector<Poly> poly::load(string filename)
{
    std::vector<Poly> ret;

    std::ifstream in(filename);
    if ( !in ) throw "Cannot open " + filename;

    while (1)
    {
        Poly cp;
        ret.push_back(cp);

        Poly & p = ret.back();

        in >> p.n;
        if ( !in ) break;
        p.d = 1;
        {
            auto x = p.n;
            while (x) { p.d++; x >>= 1; }
        }
        in >> p.m;
        for ( int i = 0; i < p.n; i++ )
        {
            string s;
            in >> s;
            if ( s == ".." )
            {
                p.vc.resize(p.n, Number(0));
                break;
            }
            p.vc.emplace_back(Number(s));
        }
    }

    ret.pop_back();
    return ret;
}

std::string Poly::str() const
{
    std::ostringstream os;
    os << n << ' ' << m.str();
    for ( int i = 0; i < n; i++ ) os << ' ' << vc[i].str();
    return os.str();
}


Poly operator&(const Poly & a, const Poly & b)
{
    if ( a.m != b.m ) never;

    Poly r(a.n + b.n, a.m);

    for ( int i = 0; i < a.n; i++ )
    {
        for ( int j = 0; j < b.n; j++ )
        {
            r.vc[i + j] += a.vc[i] * b.vc[j];
            r.vc[i + j] %= r.m;
        }
    }

    return r;
}

Poly poly::reduceModHalf(const Poly & a)
{
    if ( a.n % 2 ) never;
    Poly r(a.n / 2, a.m);
    for ( int i = 0; i < r.n; i++ )
    {
        r.vc[i] = a.vc[i].submod(a.vc[i + r.n], r.m);
    }
    return r;
}

Poly poly::reduceModN(const Poly & a, int n)
{
    Poly r(n, a.m);
    for ( int i = 0; i < a.n; i++ )
    {
        int j = i % n;
        int k = i % (2 * n);

        if ( j == k ) r.vc[j] += a.vc[i];
        else r.vc[j] = r.vc[j].submod(a.vc[i], r.m);

        r.vc[j] %=  r.m;
    }
    return r;
}


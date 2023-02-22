#include "math.h"

#include <iostream>
#include <gmp.h>
#include "io.h"

using namespace std;

namespace math
{

void MathParams::init()
{
    q = 2 * k * n + 1;
    if ( !q.isPrime() ) throw "Modulus is not prime";
    barrett_2k  = clog2(q) << 1;
    barrett_md = (Number(1)<<barrett_2k) / q; // floor(  2^{barret_2k} / q )
    Ringint::setModulus(q);
    in      = Ringint::inv(n);
    rou1n   = find1NthRootOfUnity(n);
    rou2n   = find2NthRootOfUnity(n);
    irou2n  = Ringint::inv(rou2n);
    twiddle = computePowers(n, rou1n);
    psi     = computePowers(n, rou2n);
    ipsi    = computePowers(n, irou2n);
}

void MathParams::print()
{
    cout << "n          = " << n          << '\n';
    cout << "k          = " << k          << '\n';
    cout << "q          = " << q          << '\n';
    cout << "in         = " << in         << '\n';
    cout << "barrett_2k = " << barrett_2k << '\n';
    cout << "barrett_md = " << barrett_md << '\n';
    cout << "rou1n      = " << rou1n      << '\n';
    cout << "rou2n      = " << rou2n      << '\n';
    cout << "irou2n     = " << irou2n     << '\n';
    cout << "twiddle    = "; io::printShort(twiddle);
    cout << "psi        = "; io::printShort(psi);
    cout << "ipsi       = "; io::printShort(ipsi);
}

size_t bitReverse(size_t a, size_t bit_length)
{
    size_t long res = 0;

    for (size_t i = 0; i < bit_length; i++)
    {
        res <<= 1;
        res = (a & 1) | res;
        a >>= 1;
    }

    return res;
}

vector<uint64_t> calcTwiddle(uint64_t nth_rou, uint64_t modulus, uint32_t n)
{
    vector<uint64_t> twiddle;
    mpz_t base, mod;
    mpz_init_set_ui(base, nth_rou);
    mpz_init_set_ui(mod, modulus);
    for ( uint32_t i=0; i<n; i++ )
    {
        mpz_t e;
        mpz_init(e);
        mpz_powm_ui(e, base, i, mod);
        twiddle.push_back( mpz_get_ui(e) );
    }
    return twiddle;
}

bool check1NthRoot(int n, const Number & m, const Number & om)
{
    if ( om < m ) {}
    else return false;
    Number u(1);

    if (Number::powm(om, n, m) != u) return false;

    if ( n % 2 == 0 )
    {
        if (Number::powm(om, n / 2, m) == (m - 1)) return true;
    }

    // check that all powers <n !=1
    for (int j = 1; j < n; ++j)
    {
        if (Number::powm(om, j, m) == u) return false;
    }

    return true;
}

bool check2NthRoot(int n, const Number & m, const Number & psi)
{
    Number i(2), u(1), mu(m - 1);

    auto x = Number::powm(psi, n, m);
    if (x == mu)
    {
        auto om = (psi * psi) % m;
        if ( om == u ) return false;
        if ( !check1NthRoot(n, m, om) ) return false;
        return true;
    }

    return false;
}

vector<Ringint> computePolynomial(const vector<Ringint> & p, const Ringint & row2n)
{
    vector<Ringint> pp;
    Ringint rou2ni = row2n;
    for (auto & e : p)
    {
        pp.push_back(e * rou2ni);
        rou2ni *= row2n;
    }
    return pp;
}

vector<Ringint> computePowers(int n, const Ringint & base)
{
    vector<Ringint> powers;
    Ringint power = 1;
    for (int i=0; i<n; i++)
    {
        powers.push_back(power);
        power *= base;
    }
    return powers;
}

Number find1NthRoot(int n, const Number & m, bool throws)
{
    Number i(2), u(1);
    for (; i < m; ++i)
    {
        auto x = Number::powm(i, n, m);
        if (x == u)
        {
            // now check that all powers <n !=1

            if ( n % 2 == 0 )
            {
                if (Number::powm(i, n / 2, m) == u) continue;
                return i;
            }

            bool ok = true;
            for (int j = 1; j < n; ++j)
            {
                if (Number::powm(i, j, m) == u)
                {
                    ok = false;
                    break;
                }
            }
            if (ok) return i;
        }
    }

    if (throws) throw "Cannot find root";
    return Number(0);
}

Number find2NthRoot(int n, const Number & m, bool throws)
{
    Number i(2), u(1), mu(m - 1);
    for (; i < m; ++i)
    {
        auto x = Number::powm(i, n, m);
        if (x == mu)
        {
            auto om = (i * i) % m;
            if ( om == u ) continue;
            if ( !check1NthRoot(n, m, om) ) continue;
            return i;
        }
    }

    if (throws) throw "Cannot find root";
    return Number(0);
}

Ringint find1NthRootOfUnity(int n)
{
    for (Ringint z=1; bool(z); z++)
        if (is1NthRootOfUnity(z, n)) return z;
    throw "Nth root of unity not found";
}

Ringint find2NthRootOfUnity(int n)
{
    for (Ringint z=1; bool(z); z++)
        if (is2NthRootOfUnity(z, n)) return z;
    throw "2Nth root of unity not found";
}

vector<Ringint> hadamard(const vector<Ringint> & p1, const vector<Ringint> & p2)
{
    if (p1.size() != p2.size()) throw "Polynomials have different degrees during Hadamard product";
    vector<Ringint> prod;
    for (int i=0; i<p1.size(); i++) prod.push_back(p1[i] * p2[i]);
    return prod;
}

bool is1NthRootOfUnity(const Ringint & z, int n)
{
    if ( !isRootOfUnity(z, n) ) return false;
    Ringint zm = 1;
    for (int m=1; m < n; m++)
    {
        zm *= z;
        if (zm == 1) return false;
    }
    return true;
}

bool is2NthRootOfUnity(const Ringint & z, int n)
{
    Ringint qm1 = Ringint::getModulus() - 1;
    if ( Ringint::pow(z, n) != qm1 ) return false;
    Ringint zm = 1;
    for (int m=1; m < n; m++)
    {
        zm *= z;
        if (zm == 1) return false;
    }
    return true;
}

bool isRootOfUnity(const Ringint & z, int n)
{
    return Ringint::pow(z, n) == 1;
}

uint64_t modInverse(uint64_t number, uint64_t modulus)
{
    mpz_t n, m, inv;
    mpz_init(inv);
    mpz_init_set_ui(n, number);
    mpz_init_set_ui(m, modulus);
    mpz_invert(inv, n, m);
    gmp_printf("%Zx %Zx %Zx\n", n, m, inv);
    return uint64_t( mpz_get_ui(inv) );
}

vector<Number> primitiveRoots(const Number & modulus, int cmul, int nRoots)
{
    vector<Number> roots;
    int hit = 0;
    for ( Number g=1; g<modulus; g++ )
    {
        if (hit == nRoots) break;
        auto gcmul = Number::pow(g, cmul);
        Number powm = 1;
        for ( Number powers=1; powers<modulus; powers++ )
        {
            // auto powm = Number::powm(g, powers, modulus);
            powm = (powm * g) % modulus;
            if (powm != 1) continue;
            if (powers != modulus-1) break;
            if (gcmul < modulus)
            {
                roots.push_back(gcmul);
                hit++;
            }
        }
    }
    return roots;
}

vector<uint64_t> primetiveRoots(uint64_t modulus, uint64_t cmul, size_t nroots)
{
    mpz_t mod;
    mpz_init_set_ui(mod, modulus);
    vector<uint64_t> roots;
    size_t hit = 0;
    for ( uint64_t g=1; g<modulus; g++ )
    {
        if ( hit == nroots ) break;
        mpz_t base;
        mpz_init_set_ui(base, g);
        for ( uint64_t powers=1; powers<modulus; powers++ )
        {
            mpz_t powmod;
            mpz_init(powmod);
            mpz_powm_ui(powmod, base, powers, mod);
            if ( mpz_get_ui(powmod) == 1 )
            {
                if ( powers == modulus-1 )
                {
                    mpz_t root;
                    mpz_init(root);
                    mpz_pow_ui(root, base, cmul);
                    if ( mpz_cmp(root, mod) < 0 )
                    {
                        roots.push_back( mpz_get_ui(root) );
                        hit++;
                    }
                    mpz_clear(root);
                }
                else break;
            }
            mpz_clear(powmod);
        }
        mpz_clear(base);
    }
    return roots;
}

vector<Ringint> randomRingintPolynomial(int n)
{
    vector<Ringint> p;
    for (int i=0; i<n; i++) p.push_back( Ringint::random() );
    return p;
}

vector<uint64_t> reverseTwiddle(const vector<uint64_t> & twiddle)
{
    auto size = twiddle.size();
    vector<uint64_t> revTwiddle(size);
    for ( size_t i=0; i<size; i++ )
        revTwiddle[ bitReverse(i,size) ] = twiddle[i];
    return revTwiddle;
}

} // math

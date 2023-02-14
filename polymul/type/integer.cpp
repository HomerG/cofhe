#include "integer.hpp"

using namespace integer;
using namespace std;

// static variables
bool Integer::isInitialized = false;
gmp_randstate_t Integer::state;
int Integer::primeLevel = 1;

// private functions
inline void Integer::init_random()
{
    if (isInitialized) return;
    isInitialized = true;
#ifdef MERSENNE_TWISTER
    gmp_randinit_mt(state); // Mersenne Twister algorithm (fast)
#else
    gmp_randinit_default(state);
#endif
}

inline Integer & Integer::operate(const Integer & a, const Operation & op)
{
    switch (op)
    {
        case ADD : mpz_add   (x, x, a.x);        break;
        case AND : mpz_and   (x, x, a.x);        break;
        case DIV : mpz_tdiv_q(x, x, a.x);        break;
        case GCD : mpz_gcd   (x, x, a.x);        break;
        case INV :
        {
            auto exist = mpz_invert(x, x, a.x);
            if (!exist) throw "Inverse does not exist";
            break;
        }
        case LCM : mpz_lcm   (x, x, a.x);        break;
        case MOD : mpz_mod   (x, x, a.x);        break;
        case MUL : mpz_mul   (x, x, a.x);        break;
        case OR  : mpz_ior   (x, x, a.x);        break;
        case POW : *this = pow((uint64_t)(a));   break;
        case SLL : *this <<= (uint64_t)(a);      break;
        case SLR : *this >>= (uint64_t)(a);      break;
        case SUB : mpz_sub   (x, x, a.x);        break;
        case XOR : mpz_xor   (x, x, a.x);        break;
        default  : throw "Integer operation not supported";
    }
    return *this;
}

inline Integer & Integer::operate(uint64_t a, const Operation & op)
{
    switch (op)
    {
        case ADD : mpz_add_ui     (x, x, a); break;
        case AND : *this &= Integer(a);      break;
        case DIV : mpz_tdiv_q_ui  (x, x, a); break;
        case MOD : mpz_mod_ui     (x, x, a); break;
        case MUL : mpz_mul_ui     (x, x, a); break;
        case OR  : *this |= Integer(a);      break;
        case POW : mpz_pow_ui     (x, x, a); break;
        case SLL : mpz_mul_2exp   (x, x, a); break;
        case SLR : mpz_tdiv_q_2exp(x, x, a); break;
        case SUB : mpz_sub_ui     (x, x, a); break;
        case XOR : *this ^= Integer(a);      break;
        default  : throw "Integer operation not supported";
    }
    return *this;
}

inline Integer Integer::operate_const(const Integer & a, const Operation & op) const
{
    Integer r = *this;
    r.operate(a, op);
    return r;
}

inline Integer Integer::operate_const(uint64_t a, const Operation & op) const
{
    Integer r = *this;
    r.operate(a, op);
    return r;
}

inline bool Integer::operate_logical(const Integer & a, const Logical & op) const
{
    switch (op)
    {
        case EQ  : return mpz_cmp(x, a.x)==0;
        case GE  : return mpz_cmp(x, a.x)>=0;
        case GT  : return mpz_cmp(x, a.x)> 0;
        case LAN : return bool(*this) && bool(a);
        case LE  : return mpz_cmp(x, a.x)<=0;
        case LOR : return bool(*this) || bool(a);
        case LT  : return mpz_cmp(x, a.x)< 0;
        case NE  : return mpz_cmp(x, a.x)!=0;
        default  : throw "Integer operation not supported";
    }
}

inline bool Integer::operate_logical(uint64_t a, const Logical & op) const
{
    switch (op)
    {
        case EQ  : return mpz_cmp_ui(x, a)==0;
        case GE  : return mpz_cmp_ui(x, a)>=0;
        case GT  : return mpz_cmp_ui(x, a)> 0;
        case LAN : return bool(*this) && bool(a);
        case LE  : return mpz_cmp_ui(x, a)<=0;
        case LOR : return bool(*this) || bool(a);
        case LT  : return mpz_cmp_ui(x, a)< 0;
        case NE  : return mpz_cmp_ui(x, a)!=0;
        default  : throw "Integer operation not supported";
    }
}

inline Integer Integer::operate_static(const Integer & a, const Integer & b, const Operation & op)
{
    Integer r = a;
    switch (op)
    {
        case GCD : r.gcd(b); break;
        case INV : r.inv(b); break;
        case LCM : r.lcm(b); break;
        case POW : r.pow(b); break;
        default  : throw "Integer operation not supported";
    }
    return r;
}

// constructors
Integer::Integer(const char * a, int base) { mpz_init_set_str(x, a, base); }
Integer::Integer(const mpz_t & a) { mpz_init_set(x, a); }
Integer::~Integer() { mpz_clear(x); }

// casting operators
Integer::Integer(uint64_t a) { mpz_init_set_ui(x, a); }
Integer::operator bool() const { return bool( mpz_cmp_ui(x, 0) ); }
Integer::operator string() const { return str(DEFAULT_BASE); }
Integer::operator uint64_t() const { return mpz_get_ui(x); }

// Integer & operators (const Integer &)
Integer & Integer::operator  =(const Integer & a) { mpz_set(x, a.x); return *this; }
Integer & Integer::operator +=(const Integer & a) { return operate(a, ADD); }
Integer & Integer::operator -=(const Integer & a) { return operate(a, SUB); }
Integer & Integer::operator *=(const Integer & a) { return operate(a, MUL); }
Integer & Integer::operator /=(const Integer & a) { return operate(a, DIV); }
Integer & Integer::operator %=(const Integer & a) { return operate(a, MOD); }
Integer & Integer::operator<<=(const Integer & a) { return operate(a, SLL); }
Integer & Integer::operator>>=(const Integer & a) { return operate(a, SLR); }
Integer & Integer::operator &=(const Integer & a) { return operate(a, AND); }
Integer & Integer::operator |=(const Integer & a) { return operate(a, OR ); }
Integer & Integer::operator ^=(const Integer & a) { return operate(a, XOR); }
Integer & Integer::gcd        (const Integer & a) { return operate(a, GCD); }
Integer & Integer::inv        (const Integer & a) { return operate(a, INV); }
Integer & Integer::lcm        (const Integer & a) { return operate(a, LCM); }
Integer & Integer::pow        (const Integer & a) { return operate(a, POW); }

Integer & Integer::powm(const Integer & exp, const Integer & mod)
{
    mpz_powm(x, x, exp.x, mod.x);
    return *this;
}

// Integer & operators (uint64_t)
Integer & Integer::operator++() { return *this += 1; }
Integer & Integer::operator--() { return *this -= 1; }
Integer & Integer::operator +=(uint64_t a) { return operate(a, ADD); }
Integer & Integer::operator -=(uint64_t a) { return operate(a, SUB); }
Integer & Integer::operator *=(uint64_t a) { return operate(a, MUL); }
Integer & Integer::operator /=(uint64_t a) { return operate(a, DIV); }
Integer & Integer::operator %=(uint64_t a) { return operate(a, MOD); }
Integer & Integer::operator<<=(uint64_t a) { return operate(a, SLL); }
Integer & Integer::operator>>=(uint64_t a) { return operate(a, SLR); }
Integer & Integer::operator &=(uint64_t a) { return operate(a, AND); }
Integer & Integer::operator |=(uint64_t a) { return operate(a, OR ); }
Integer & Integer::operator ^=(uint64_t a) { return operate(a, XOR); }
Integer & Integer::pow        (uint64_t a) { return operate(a, POW); }

Integer & Integer::powm(uint64_t exp, const Integer & mod)
{
    mpz_powm_ui(x, x, exp, mod.x);
    return *this;
}

// Integer operators (const Integer &) const
Integer Integer::operator +(const Integer & a) const { return operate_const(a, ADD); }
Integer Integer::operator -(const Integer & a) const { return operate_const(a, SUB); }
Integer Integer::operator *(const Integer & a) const { return operate_const(a, MUL); }
Integer Integer::operator /(const Integer & a) const { return operate_const(a, DIV); }
Integer Integer::operator %(const Integer & a) const { return operate_const(a, MOD); }
Integer Integer::operator<<(const Integer & a) const { return operate_const(a, SLL); }
Integer Integer::operator>>(const Integer & a) const { return operate_const(a, SLR); }
Integer Integer::operator &(const Integer & a) const { return operate_const(a, AND); }
Integer Integer::operator |(const Integer & a) const { return operate_const(a, OR ); }
Integer Integer::operator ^(const Integer & a) const { return operate_const(a, XOR); }

// Integer operators (uint64_t) const
Integer Integer::operator++(int) { Integer r = *this; ++(*this); return r; }
Integer Integer::operator--(int) { Integer r = *this; --(*this); return r; }
Integer Integer::operator +(uint64_t a) const { return operate_const(a, ADD); }
Integer Integer::operator -(uint64_t a) const { return operate_const(a, SUB); }
Integer Integer::operator *(uint64_t a) const { return operate_const(a, MUL); }
Integer Integer::operator /(uint64_t a) const { return operate_const(a, DIV); }
Integer Integer::operator %(uint64_t a) const { return operate_const(a, MOD); }
Integer Integer::operator<<(uint64_t a) const { return operate_const(a, SLL); }
Integer Integer::operator>>(uint64_t a) const { return operate_const(a, SLR); }
Integer Integer::operator &(uint64_t a) const { return operate_const(a, AND); }
Integer Integer::operator |(uint64_t a) const { return operate_const(a, OR ); }
Integer Integer::operator ^(uint64_t a) const { return operate_const(a, XOR); }

bool Integer::operator[](int a) const { return mpz_tstbit(x, a); };
bool Integer::operator !() const { return !bool(*this); }
bool Integer::operator&&(const Integer & a) const { return operate_logical(a, LAN); }
bool Integer::operator||(const Integer & a) const { return operate_logical(a, LOR); }
bool Integer::operator==(const Integer & a) const { return operate_logical(a, EQ ); }
bool Integer::operator!=(const Integer & a) const { return operate_logical(a, NE ); }
bool Integer::operator<=(const Integer & a) const { return operate_logical(a, LE ); }
bool Integer::operator< (const Integer & a) const { return operate_logical(a, LT ); }
bool Integer::operator>=(const Integer & a) const { return operate_logical(a, GE ); }
bool Integer::operator> (const Integer & a) const { return operate_logical(a, GT ); }

// bool operators (uint64_t) const
bool Integer::operator&&(uint64_t a) const { return operate_logical(a, LAN); }
bool Integer::operator||(uint64_t a) const { return operate_logical(a, LOR); }
bool Integer::operator==(uint64_t a) const { return operate_logical(a, EQ ); }
bool Integer::operator!=(uint64_t a) const { return operate_logical(a, NE ); }
bool Integer::operator<=(uint64_t a) const { return operate_logical(a, LE ); }
bool Integer::operator< (uint64_t a) const { return operate_logical(a, LT ); }
bool Integer::operator>=(uint64_t a) const { return operate_logical(a, GE ); }
bool Integer::operator> (uint64_t a) const { return operate_logical(a, GT ); }
bool Integer::isPrime() const { return mpz_probab_prime_p(x, DEFAULT_REPETITIONS) >= primeLevel; }

// static functions
Integer Integer::gcd(const Integer & a, const Integer & b) { return operate_static(a, b, GCD); }
Integer Integer::inv(const Integer & a, const Integer & b) { return operate_static(a, b, INV); }
Integer Integer::lcm(const Integer & a, const Integer & b) { return operate_static(a, b, LCM); }
Integer Integer::pow(const Integer & a, const Integer & b) { return operate_static(a, b, POW); }

Integer Integer::pow(const Integer & base, uint64_t exp)
{
    Integer r = base;
    r.pow(exp);
    return r;
}

Integer Integer::powm(const Integer & base, const Integer & exp, const Integer & mod)
{
    Integer r = base;
    r.powm(exp, mod);
    return r;
}

Integer Integer::powm(const Integer & base, uint64_t exp, const Integer & mod)
{
    Integer r = base;
    r.powm(exp, mod);
    return r;
}

Integer Integer::nextPrime(const Integer & a)
{
    Integer r;
    mpz_nextprime(r.x, a.x);
    return r;
}

// generate a random Integer in the interval [0, n-1]
Integer Integer::random(const Integer & n)
{
    Integer r;
    init_random();
    mpz_urandomm(r.x, state, n.x);
    return r;
}

// generate a random Integer in the interval [0, 2^n-1]
Integer Integer::random(uint64_t n)
{
    Integer r;
    init_random();
    mpz_urandomb(r.x, state, n);
    return r;
}

Integer Integer::randomPrime(const Integer & n)
{
    Integer r;
    do { mpz_nextprime(r.x, random(n).x); }
    while (r >= n);
    return r;
}

Integer Integer::randomPrime(uint64_t logn)
{
    auto n = Integer(1) << logn;
    Integer r;
    do { mpz_nextprime(r.x, random(logn).x); }
    while (r >= n);
    return r;
}

bool Integer::isPrime(const Integer & a) { return a.isPrime(); }

void Integer::setPrimeLevel(int primeLevel) { Integer::primeLevel = primeLevel; }

// functions
string Integer::str(int base) const { return mpz_get_str(NULL, base, x); }
ostream & operator<<(ostream & os, const Integer & a) { return os << string(a); }

// friend operators

Integer operator +(uint64_t a, const Integer & b) { return Integer(a) + b; }
Integer operator -(uint64_t a, const Integer & b) { return Integer(a) - b; }
Integer operator *(uint64_t a, const Integer & b) { return Integer(a) * b; }
Integer operator /(uint64_t a, const Integer & b) { return Integer(a) / b; }
Integer operator %(uint64_t a, const Integer & b) { return Integer(a) % b; }
Integer operator<<(uint64_t a, const Integer & b) { return Integer(a)<< b; }
Integer operator>>(uint64_t a, const Integer & b) { return Integer(a)>> b; }
Integer operator &(uint64_t a, const Integer & b) { return Integer(a) & b; }
Integer operator |(uint64_t a, const Integer & b) { return Integer(a) | b; }
Integer operator ^(uint64_t a, const Integer & b) { return Integer(a) ^ b; }

bool operator &&(uint64_t a, const Integer & b) { return Integer(a) && b; }
bool operator ||(uint64_t a, const Integer & b) { return Integer(a) || b; }
bool operator ==(uint64_t a, const Integer & b) { return Integer(a) == b; }
bool operator !=(uint64_t a, const Integer & b) { return Integer(a) != b; }
bool operator >=(uint64_t a, const Integer & b) { return Integer(a) >= b; }
bool operator > (uint64_t a, const Integer & b) { return Integer(a) >  b; }
bool operator <=(uint64_t a, const Integer & b) { return Integer(a) <= b; }
bool operator < (uint64_t a, const Integer & b) { return Integer(a) <  b; }

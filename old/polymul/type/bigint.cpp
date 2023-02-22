#include "bigint.hpp"

using namespace std;

Bigint::operator bool() const { return x != e3::Bigun(0); }
Bigint::operator uint64_t() const { return uint64_t(x.ull()); }
Bigint::operator string() const { return str(); }

Bigint & Bigint::operator +=(const Bigint & a) { x += a.x; return *this; }
Bigint & Bigint::operator -=(const Bigint & a) { x -= a.x; return *this; }
Bigint & Bigint::operator *=(const Bigint & a) { x *= a.x; return *this; }
Bigint & Bigint::operator /=(const Bigint & a) { x /= a.x; return *this; }
Bigint & Bigint::operator %=(const Bigint & a) { x %= a.x; return *this; }
Bigint & Bigint::operator<<=(const Bigint & a) { x<<= a.x; return *this; }
Bigint & Bigint::operator>>=(const Bigint & a) { x>>= a.x; return *this; }
Bigint & Bigint::operator &=(const Bigint & a) { x &= a.x; return *this; }
Bigint & Bigint::operator |=(const Bigint & a) { x |= a.x; return *this; }
Bigint & Bigint::operator ^=(const Bigint & a) { x ^= a.x; return *this; }
Bigint & Bigint::gcd(const Bigint & a) { return *this; } // FIXME
Bigint & Bigint::inv(const Bigint & a) { x = x.invmod(a.x); return *this; }
Bigint & Bigint::lcm(const Bigint & a) { return *this; } // FIXME
Bigint & Bigint::pow(const Bigint & exp, const Bigint & mod)
{ x = x.powmod(exp.x, mod.x); return *this; }

Bigint & Bigint::operator++() { ++x; return *this; }
Bigint & Bigint::operator--() { --x; return *this; }
// Bigint & Bigint::operator +=(uint64_t a) { x += ull(a); return *this; }
// Bigint & Bigint::operator -=(uint64_t a) { x -= a; return *this; }
// Bigint & Bigint::operator *=(uint64_t a) { x *= a; return *this; }
// Bigint & Bigint::operator /=(uint64_t a) { x /= a; return *this; }
// Bigint & Bigint::operator %=(uint64_t a) { x %= a; return *this; }
// Bigint & Bigint::operator<<=(uint64_t a) { x<<= a; return *this; }
// Bigint & Bigint::operator>>=(uint64_t a) { x>>= a; return *this; }
// Bigint & Bigint::operator &=(uint64_t a) { x &= a; return *this; }
// Bigint & Bigint::operator |=(uint64_t a) { x |= a; return *this; }
// Bigint & Bigint::operator ^=(uint64_t a) { x ^= a; return *this; }
// Bigint & Bigint::pow(uint64_t exp, const Bigint & mod)
// { return *this; } // FIXME

Bigint Bigint::operator +(const Bigint & a) const { Bigint r = *this; r += a; return r; }
Bigint Bigint::operator -(const Bigint & a) const { Bigint r = *this; r -= a; return r; }
Bigint Bigint::operator *(const Bigint & a) const { Bigint r = *this; r *= a; return r; }
Bigint Bigint::operator /(const Bigint & a) const { Bigint r = *this; r /= a; return r; }
Bigint Bigint::operator %(const Bigint & a) const { Bigint r = *this; r %= a; return r; }
Bigint Bigint::operator<<(const Bigint & a) const { Bigint r = *this; r<<= a; return r; }
Bigint Bigint::operator>>(const Bigint & a) const { Bigint r = *this; r>>= a; return r; }
Bigint Bigint::operator &(const Bigint & a) const { Bigint r = *this; r &= a; return r; }
Bigint Bigint::operator |(const Bigint & a) const { Bigint r = *this; r |= a; return r; }
Bigint Bigint::operator ^(const Bigint & a) const { Bigint r = *this; r ^= a; return r; }

Bigint Bigint::operator++(int) { Bigint r = *this; ++(*this); return r; }
Bigint Bigint::operator--(int) { Bigint r = *this; --(*this); return r; }
// Bigint Bigint::operator +(uint64_t a) const { Bigint r = *this; r += a; return r; }
// Bigint Bigint::operator -(uint64_t a) const { Bigint r = *this; r -= a; return r; }
// Bigint Bigint::operator *(uint64_t a) const { Bigint r = *this; r *= a; return r; }
// Bigint Bigint::operator /(uint64_t a) const { Bigint r = *this; r /= a; return r; }
// Bigint Bigint::operator %(uint64_t a) const { Bigint r = *this; r %= a; return r; }
// Bigint Bigint::operator<<(uint64_t a) const { Bigint r = *this; r<<= a; return r; }
// Bigint Bigint::operator>>(uint64_t a) const { Bigint r = *this; r>>= a; return r; }
// Bigint Bigint::operator &(uint64_t a) const { Bigint r = *this; r &= a; return r; }
// Bigint Bigint::operator |(uint64_t a) const { Bigint r = *this; r |= a; return r; }
// Bigint Bigint::operator ^(uint64_t a) const { Bigint r = *this; r ^= a; return r; }

bool Bigint::operator[](int) const { return false; } // FIXME
bool Bigint::operator !() const { return !x; }
bool Bigint::operator&&(const Bigint & a) const { return x && a.x; }
bool Bigint::operator||(const Bigint & a) const { return x || a.x; }
bool Bigint::operator==(const Bigint & a) const { return x == a.x; }
bool Bigint::operator!=(const Bigint & a) const { return x != a.x; }
bool Bigint::operator>=(const Bigint & a) const { return x >= a.x; }
bool Bigint::operator> (const Bigint & a) const { return x >  a.x; }
bool Bigint::operator<=(const Bigint & a) const { return x <= a.x; }
bool Bigint::operator< (const Bigint & a) const { return x <  a.x; }

// bool Bigint::operator&&(uint64_t a) const { return x && a; }
// bool Bigint::operator||(uint64_t a) const { return x || a; }
// bool Bigint::operator==(uint64_t a) const { return x == a; }
// bool Bigint::operator!=(uint64_t a) const { return x != a; }
// bool Bigint::operator>=(uint64_t a) const { return x >= a; }
// bool Bigint::operator> (uint64_t a) const { return x >  a; }
// bool Bigint::operator<=(uint64_t a) const { return x <= a; }
// bool Bigint::operator< (uint64_t a) const { return x <  a; }

Bigint Bigint::gcd(const Bigint & a, const Bigint & b)
{ Bigint r = a; return r.gcd(b); }
Bigint Bigint::inv(const Bigint & a, const Bigint & b)
{ Bigint r = a; return r.inv(b); }
Bigint Bigint::lcm(const Bigint & a, const Bigint & b)
{ Bigint r = a; return r.lcm(b); }
Bigint Bigint::pow(const Bigint & base, const Bigint & exp, const Bigint & mod)
{ Bigint r = base; return r.pow(exp, mod); }
// Bigint Bigint::pow(const Bigint & base, uint64_t exp, const Bigint & mod);
// { Bigint r = base; return r.pow(exp, mod); }

string Bigint::str(int base) const { return x.str(); }
ostream & operator<<(ostream & os, const Bigint & a) { return os << string(a); }

Bigint operator +(uint64_t a, const Bigint & b) { return Bigint(a) + b; }
Bigint operator -(uint64_t a, const Bigint & b) { return Bigint(a) - b; }
Bigint operator *(uint64_t a, const Bigint & b) { return Bigint(a) * b; }
Bigint operator /(uint64_t a, const Bigint & b) { return Bigint(a) / b; }
Bigint operator %(uint64_t a, const Bigint & b) { return Bigint(a) % b; }
Bigint operator<<(uint64_t a, const Bigint & b) { return Bigint(a)<< b; }
Bigint operator>>(uint64_t a, const Bigint & b) { return Bigint(a)>> b; }
Bigint operator &(uint64_t a, const Bigint & b) { return Bigint(a) & b; }
Bigint operator |(uint64_t a, const Bigint & b) { return Bigint(a) | b; }
Bigint operator ^(uint64_t a, const Bigint & b) { return Bigint(a) ^ b; }

bool operator &&(uint64_t a, const Bigint & b) { return Bigint(a) && b; }
bool operator ||(uint64_t a, const Bigint & b) { return Bigint(a) || b; }
bool operator ==(uint64_t a, const Bigint & b) { return Bigint(a) == b; }
bool operator !=(uint64_t a, const Bigint & b) { return Bigint(a) != b; }
bool operator >=(uint64_t a, const Bigint & b) { return Bigint(a) >= b; }
bool operator > (uint64_t a, const Bigint & b) { return Bigint(a) >  b; }
bool operator <=(uint64_t a, const Bigint & b) { return Bigint(a) <= b; }
bool operator < (uint64_t a, const Bigint & b) { return Bigint(a) <  b; }

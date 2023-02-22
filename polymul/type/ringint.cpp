#include "ringint.hpp"

using namespace ringint;
using namespace std;

// static variables
Number Ringint::mod = 2;
// bool Ringint::isPrimitiveRoot = false;
// bool Ringint::isRoot = false;
// vector<Number> Ringint::primitiveRoots;
// vector<Number> Ringint::roots;
// vector<Number> Ringint::iroots;

// private functions

Ringint & Ringint::operate(const Ringint & a, const Operator & op)
{
    switch (op)
    {
        case ADD : x += a.x;                   break;
        case DIV : x *= Number::inv(a.x, mod); break; // multiply by the inverse of 'a'
        case GCD : x.gcd(a.x);                 break;
        case LCM : x.lcm(a.x);                 break;
        case MOD : x %= a.x;                   break;
        case MUL : x *= a.x;                   break;
        case POW : x.powm(a.x, mod);           break;
        case SUB : x = x + ( x < a.x ? mod : Number(0) ) - a.x; break;
        default  : throw "Ringint operation not supported";
    }
    x %= mod;
    return *this;
}

Ringint Ringint::operate_const(const Ringint & a, const Operator & op) const
{
    Ringint r = *this;
    r.operate(a, op);
    return r;
}

bool Ringint::operate_logical(const Ringint & a, const Logical & op) const
{
    switch (op)
    {
        case EQ : return x == a.x;
        case GE : return x >= a.x;
        case GT : return x >  a.x;
        case LE : return x <= a.x;
        case LT : return x <  a.x;
        case NE : return x != a.x;
        default : throw "Ringint operation not supported";
    }
}

Ringint Ringint::operate_static(const Ringint & a, const Ringint & b, const Operator & op)
{
    Ringint r = a;
    switch (op)
    {
        case GCD : r.gcd(b); break;
        case LCM : r.lcm(b); break;
        case POW : r.pow(b); break;
        default  : throw "Ringint operation not supported";
    }
    return r;
}

// constructors

Ringint::Ringint(const Number & a)
{
    x = a;
    x %= mod;
}

// casting operators
Ringint::operator bool() const { return bool(x); }
Ringint::operator string() const { return str(); }
Ringint::operator uint64_t() const { return uint64_t(x); }

// Ringint & operators
Ringint & Ringint::operator +=(const Ringint & a) { return operate(a, ADD); }
Ringint & Ringint::operator -=(const Ringint & a) { return operate(a, SUB); }
Ringint & Ringint::operator *=(const Ringint & a) { return operate(a, MUL); }
Ringint & Ringint::operator /=(const Ringint & a) { return operate(a, DIV); }
Ringint & Ringint::operator %=(const Ringint & a) { return operate(a, MOD); }
Ringint & Ringint::gcd        (const Ringint & a) { return operate(a, GCD); }
Ringint & Ringint::lcm        (const Ringint & a) { return operate(a, LCM); }
Ringint & Ringint::pow        (const Ringint & a) { return operate(a, POW); }
Ringint & Ringint::operator++() { return *this += 1; }
Ringint & Ringint::operator--() { return *this -= 1; }

Ringint & Ringint::inv()
{
    x.inv( mod );
    return *this;
}

// Ringint & Ringint::operator +=(uint64_t a) { return *this += Ringint(a, mod); }
// Ringint & Ringint::operator -=(uint64_t a) { return *this += Ringint(a, mod); }
// Ringint & Ringint::operator *=(uint64_t a) { return *this += Ringint(a, mod); }
// Ringint & Ringint::operator /=(uint64_t a) { return *this += Ringint(a, mod); }
// Ringint & Ringint::operator %=(uint64_t a) { return *this += Ringint(a, mod); }

// Ringint operators
Ringint Ringint::operator++(int) { Ringint r = *this; ++(*this); return r; }
Ringint Ringint::operator--(int) { Ringint r = *this; --(*this); return r; }
Ringint Ringint::operator +(const Ringint & a) const { return operate_const(a, ADD); }
Ringint Ringint::operator -(const Ringint & a) const { return operate_const(a, SUB); }
Ringint Ringint::operator *(const Ringint & a) const { return operate_const(a, MUL); }
Ringint Ringint::operator /(const Ringint & a) const { return operate_const(a, DIV); }
Ringint Ringint::operator %(const Ringint & a) const { return operate_const(a, MOD); }

// Ringint Ringint::operator +(uint64_t a) const { return *this + Ringint(a, mod); }
// Ringint Ringint::operator -(uint64_t a) const { return *this - Ringint(a, mod); }
// Ringint Ringint::operator *(uint64_t a) const { return *this * Ringint(a, mod); }
// Ringint Ringint::operator /(uint64_t a) const { return *this / Ringint(a, mod); }
// Ringint Ringint::operator %(uint64_t a) const { return *this % Ringint(a, mod); }

// bool operators
bool Ringint::operator !() const { return !bool(*this); }
bool Ringint::operator==(const Ringint & a) const { return operate_logical(a, EQ); }
bool Ringint::operator!=(const Ringint & a) const { return operate_logical(a, NE); }
bool Ringint::operator<=(const Ringint & a) const { return operate_logical(a, LE); }
bool Ringint::operator< (const Ringint & a) const { return operate_logical(a, LT); }
bool Ringint::operator>=(const Ringint & a) const { return operate_logical(a, GE); }
bool Ringint::operator> (const Ringint & a) const { return operate_logical(a, GT); }

// bool Ringint::operator==(uint64_t a) const { return *this == Ringint(a, mod); }
// bool Ringint::operator!=(uint64_t a) const { return *this != Ringint(a, mod); }
// bool Ringint::operator>=(uint64_t a) const { return *this >= Ringint(a, mod); }
// bool Ringint::operator> (uint64_t a) const { return *this >  Ringint(a, mod); }
// bool Ringint::operator<=(uint64_t a) const { return *this <= Ringint(a, mod); }
// bool Ringint::operator< (uint64_t a) const { return *this <  Ringint(a, mod); }

// static functions
Ringint Ringint::gcd(const Ringint & a, const Ringint & b) { return operate_static(a, b, GCD); }
Ringint Ringint::lcm(const Ringint & a, const Ringint & b) { return operate_static(a, b, LCM); }
Ringint Ringint::pow(const Ringint & a, const Ringint & b) { return operate_static(a, b, POW); }

Ringint Ringint::inv(const Ringint & a)
{
    Ringint r = a;
    r.inv();
    return r;
}

Ringint Ringint::random() { return Number::random(mod); }

// public functions
Number Ringint::getModulus() { return mod; }
void Ringint::setModulus(const Number & a) { mod = a; }

// Ringint Ringint::primitiveRoot(int index)
// {
//     if (!isPrimitiveRoot)
//     {
//         isPrimitiveRoot = true;
//
//         vector<Number> v;
//         Number t = mod - 1;
//         for (Number i=2; i < t+1; i++)
//         {
//             if (i*i > t) break;
//             if(t % i == 0)
//             {
//                 v.push_back((mod-1) / i);
//                 while (t % i == 0) t /= i;
//             }
//         }
//
//         if(t > 1) v.push_back((mod-1) / t);
//
//         for(Number i = index+1; i < mod; i++)
//         {
//             int flag = 0;
//             for (int j=0; j < v.size(); j++)
//             {
//                 if (math::powm(i, v[j], mod) == 1)
//                 {
//                     flag = 1;
//                     break;
//                 }
//             }
//             if (flag == 0) return i;
//         }
//     }
//     return index < primitiveRoots.size() ? Ringint(primitiveRoots[index]) : Ringint(0);
// }

string Ringint::str(int base) const { return x.str(base); }
ostream & operator<<(ostream & os, const Ringint & a) { return os << string(a); }

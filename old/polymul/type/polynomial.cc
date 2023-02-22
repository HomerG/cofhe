#pragma once

// private functions

template <class T> void
Polynomial<T>::compact() { poly.resize( order()+1 ); }

template <class T> bool
Polynomial<T>::compare(const Polynomial & a, const polynomial::Logical & op) const
{
    return polynomial::compare(poly, a.poly, op);
}

template <class T> Polynomial<T>
Polynomial<T>::operate(const Polynomial<T> & a, const polynomial::Operator & op) const
{
    auto r = *this;
    return r.operate_inplace(a, op);
}

template <class T> Polynomial<T>
Polynomial<T>::operate(const T & a, const polynomial::Operator & op) const
{
    auto r = *this;
    return r.operate_inplace(a, op);
}

template <class T> Polynomial<T> &
Polynomial<T>::operate_inplace(const Polynomial<T> & a, const polynomial::Operator & op)
{
    switch (op)
    {
        case polynomial::POLYDIV : polynomial::divide_inplace  (poly, a.poly); break;
        case polynomial::POLYMUL : polynomial::multiply_inplace(poly, a.poly); break;
        case polynomial::POLYRED : polynomial::reduce_inplace  (poly, a.poly); break;
        default                  : polynomial::operate_inplace (poly, a.poly, op);
    }
    compact();
    return *this;
}

template <class T> Polynomial<T> &
Polynomial<T>::operate_inplace(const T & a, const polynomial::Operator & op)
{
    polynomial::operate_inplace(poly, a, op);
    return *this;
}

template <class T> Polynomial<T>
Polynomial<T>::rotate(int a) const
{
    auto r = *this;
    return r.rotate_inplace(a);
}

template <class T> Polynomial<T> &
Polynomial<T>::rotate_inplace(int a)
{
    auto size = poly.size();
    if (a < 0) a += size;
    std::vector<T> tmp;
    tmp.assign(poly.begin(), poly.end());
    for (int i=0; i<size; i++)
        poly[ (i + a) % size ] = tmp[i];
    return *this;
}

// constructor

template <class T>
Polynomial<T>::Polynomial(const std::vector<T> & a) { poly.assign(a.begin(), a.end()); compact(); }

// casting operators
template <class T>
Polynomial<T>::operator std::string() const
{
    std::string str;
    for (auto & e : poly) str += std::string(e) + " ";
    if ( !str.empty() ) str = str.substr(0, str.size()-1);
    return str;
}

// operators

template <class T> Polynomial<T> &
Polynomial<T>::operator *=(const Polynomial & a) { return operate_inplace(a, polynomial::POLYMUL); }

template <class T> Polynomial<T> &
Polynomial<T>::operator /=(const Polynomial & a) { return operate_inplace(a, polynomial::POLYDIV); }

template <class T> Polynomial<T> &
Polynomial<T>::operator %=(const Polynomial & a) { return operate_inplace(a, polynomial::POLYRED); }

template <class T> Polynomial<T> &
Polynomial<T>::operator +=(const Polynomial & a) { return operate_inplace(a, polynomial::ADD); }

template <class T> Polynomial<T> &
Polynomial<T>::operator |=(const Polynomial & a) { return operate_inplace(a, polynomial::DIV); }

template <class T> Polynomial<T> &
Polynomial<T>::operator &=(const Polynomial & a) { return operate_inplace(a, polynomial::MUL); }

template <class T> Polynomial<T> &
Polynomial<T>::operator -=(const Polynomial & a) { return operate_inplace(a, polynomial::SUB); }

template <class T> Polynomial<T> &
Polynomial<T>::operator +=(const T & a) { return operate_inplace(a, polynomial::ADD); }

template <class T> Polynomial<T> &
Polynomial<T>::operator /=(const T & a) { return operate_inplace(a, polynomial::DIV); }

template <class T> Polynomial<T> &
Polynomial<T>::operator %=(const T & a) { return operate_inplace(a, polynomial::MOD); }

template <class T> Polynomial<T> &
Polynomial<T>::operator *=(const T & a) { return operate_inplace(a, polynomial::MUL); }

template <class T> Polynomial<T> &
Polynomial<T>::operator -=(const T & a) { return operate_inplace(a, polynomial::SUB); }

template <class T> T &
Polynomial<T>::operator [](int a)
{
    if ( a < 0 ) throw "The index must be non negative";
    if ( a >= poly.size() ) poly.resize( a+1, T(0) );
    return poly[a];
}

template <class T> Polynomial<T> &
Polynomial<T>::operator<<=(int a) { return rotate_inplace(-a); }

template <class T> Polynomial<T> &
Polynomial<T>::operator>>=(int a) { return rotate_inplace(a); }

template <class T> Polynomial<T>
Polynomial<T>::operator * (const Polynomial & a) const { return operate(a, polynomial::POLYMUL); }

template <class T> Polynomial<T>
Polynomial<T>::operator / (const Polynomial & a) const { return operate(a, polynomial::POLYDIV); }

template <class T> Polynomial<T>
Polynomial<T>::operator % (const Polynomial & a) const { return operate(a, polynomial::POLYRED); }

template <class T> Polynomial<T>
Polynomial<T>::operator + (const Polynomial & a) const { return operate(a, polynomial::ADD); }

template <class T> Polynomial<T>
Polynomial<T>::operator | (const Polynomial & a) const { return operate(a, polynomial::DIV); }

template <class T> Polynomial<T>
Polynomial<T>::operator & (const Polynomial & a) const { return operate(a, polynomial::MUL); }

template <class T> Polynomial<T>
Polynomial<T>::operator - (const Polynomial & a) const { return operate(a, polynomial::SUB); }

template <class T> Polynomial<T>
Polynomial<T>::operator ~ () const
{
    auto r = *this;
    T one(1);
    for (auto & e : r.poly) e = one / e;
    return r;
}

template <class T> Polynomial<T>
Polynomial<T>::operator + (const T & a) const { return operate(a, polynomial::ADD); }

template <class T> Polynomial<T>
Polynomial<T>::operator / (const T & a) const { return operate(a, polynomial::DIV); }

template <class T> Polynomial<T>
Polynomial<T>::operator % (const T & a) const { return operate(a, polynomial::MOD); }

template <class T> Polynomial<T>
Polynomial<T>::operator * (const T & a) const { return operate(a, polynomial::MUL); }

template <class T> Polynomial<T>
Polynomial<T>::operator - (const T & a) const { return operate(a, polynomial::SUB); }

template <class T> Polynomial<T>
Polynomial<T>::operator<<(int a) { return rotate(-a); }

template <class T> Polynomial<T>
Polynomial<T>::operator>>(int a) { return rotate(a); }

template <class T> bool
Polynomial<T>::operator ==(const Polynomial & a) const { return compare(a, polynomial::EQ); }

template <class T> bool
Polynomial<T>::operator >=(const Polynomial & a) const { return compare(a, polynomial::GE); }

template <class T> bool
Polynomial<T>::operator > (const Polynomial & a) const { return compare(a, polynomial::GT); }

template <class T> bool
Polynomial<T>::operator <=(const Polynomial & a) const { return compare(a, polynomial::LE); }

template <class T> bool
Polynomial<T>::operator < (const Polynomial & a) const { return compare(a, polynomial::LT); }

template <class T> bool
Polynomial<T>::operator !=(const Polynomial & a) const { return compare(a, polynomial::NE); }

// functions

template <class T>
std::vector<T> Polynomial<T>::get() const { return poly; }

template <class T>
int Polynomial<T>::order() const { return polynomial::order(poly); }

template <class T>
int Polynomial<T>::size() const { return poly.size(); }

// friend functions

template <class U> std::ostream &
operator<<(std::ostream & os, const Polynomial<U> & a) { return os << std::string(a); }

#pragma once

#include <algorithm>
#include <vector>

namespace polynomial
{

enum Operator { POLYDIV, POLYMUL, POLYRED, ADD, DIV, MOD, MUL, SUB };
enum Logical { EQ, GE, GT, LE, LT, NE };

// Return a positive value if p1 > p2, zero if p1 = p2, or a negative value if p1 < p2.
template <class T>
int compare(const std::vector<T> & poly1, const std::vector<T> & poly2)
{
    std::vector<T> p1, p2;
    p1.assign( poly1.begin(), poly1.end() );
    p2.assign( poly2.begin(), poly2.end() );
    auto size = std::max(p1.size(), p2.size());
    p1.resize(size, 0);
    p2.resize(size, 0);
    for ( int i=size-1; i>=0; i-- )
        if ( p1[i] != p2[i] ) return p1[i] > p2[i] ? 1 : -1;
    return 0;
}

template <class T>
bool compare(const std::vector<T> & poly1, const std::vector<T> & poly2, const Logical & op)
{
    switch (op)
    {
        case EQ : return compare(poly1, poly2) == 0;
        case GE : return compare(poly1, poly2) >= 0;
        case GT : return compare(poly1, poly2) >  0;
        case LE : return compare(poly1, poly2) <= 0;
        case LT : return compare(poly1, poly2) <  0;
        case NE : return compare(poly1, poly2) != 0;
    }
    throw "Logical operation not supported";
}

template <class T>
bool operator< (const std::vector<T> & v1, const std::vector<T> & v2)
{ return compare(v1, v2) <  0; }

template <class T>
bool operator>=(const std::vector<T> & v1, const std::vector<T> & v2)
{ return compare(v1, v2) >= 0; }

template <class T>
void operate_inplace(std::vector<T> & a, const T & b, const Operator & op)
{
    for (auto & e : a)
    switch (op)
    {
        case ADD : e += b; break;
        case DIV : e /= b; break;
        case MOD : e %= b; break;
        case MUL : e *= b; break;
        case SUB : e -= b; break;
        default: throw "Operator not supported";
    }
}

template <class T>
void operate_inplace(std::vector<T> & a, const std::vector<T> & b, const Operator & op)
{
    auto size = std::max(a.size(), b.size());
    std::vector<T> c;
    c.assign(b.begin(), b.end());
    a.resize(size, T(0));
    c.resize(size, T(0));
    for (int i=0; i<size; i++)
    switch (op)
    {
        case ADD : a[i] += c[i]; break;
        case DIV : a[i] /= c[i]; break;
        case MUL : a[i] *= c[i]; break;
        case SUB : a[i] -= c[i]; break;
        default: throw "Operator not supported";
    }
}

template <class T>
std::vector<T> multiply(const std::vector<T> & v, const T & k, const int order=0)
{
    std::vector<T> r(order, T(0));
    for (auto & e : v) r.push_back(e * k);
    return r;
}

template <class T>
int order(const std::vector<T> & poly)
{
    for ( int i=poly.size()-1; i > 0; i-- ) if ( poly[i] ) return i;
    return 0;
}

template <class T>
std::vector<T> subtract(const std::vector<T> & a, const std::vector<T> & b)
{
    int size = std::min(a.size(), b.size());
    std::vector<T> r;
    r.assign( a.begin(), a.end() );
    while (size--) r[size] -= b[size];
    return r;
}

template <class T>
void divide_inplace(std::vector<T> & poly, const std::vector<T> & mod)
{
    auto porder = order(poly);
    auto morder = order(mod);
    auto qorder = porder >= morder ? porder - morder + 1 : 0;
    std::vector<T> q(qorder, T(0));
    while (qorder--)
    {
        if ( poly[porder] )
        {
            auto coeff = poly[porder] / mod[morder];
            auto tmp = multiply(mod, coeff, qorder);
            operate_inplace(poly, tmp, SUB);
            q[qorder] = coeff;
        }
        porder--;
    }
    poly = q;
}

template <class T>
void multiply_inplace(std::vector<T> & a, const std::vector<T> & b)
{
    int size = a.size() + b.size();
    if (size > 0) size--;
    std::vector<T> r(size, T(0));
    for (int i=0; i<a.size(); i++)
        for (int j=0; j<b.size(); j++)
            r[i+j] += a[i] * b[j];
    a = r;
}

template <class T>
void reduce_inplace(std::vector<T> & poly, const std::vector<T> & mod)
{
    auto porder = order(poly);
    auto morder = order(mod);
    auto qorder = porder >= morder ? porder - morder + 1 : 0;
    while (qorder--)
    {
        if ( poly[porder] )
        {
            auto coeff = poly[porder] / mod[morder];
            auto tmp = multiply(mod, coeff, qorder);
            operate_inplace(poly, tmp, SUB);
        }
        porder--;
    }
}

template <class T>
std::vector<T> reduce(const std::vector<T> & poly, const std::vector<T> & mod)
{
    std::vector<T> r;
    r.assign(poly.begin(), poly.end());
    reduce_inplace(r, mod);
    return r;
}

} // polynomial

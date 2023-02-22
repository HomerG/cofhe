#pragma once

#include <ostream>
#include <string>
#include <vector>
#include "vector_polynomial.h"

template <class T>
class Polynomial
{
    private:
        std::vector<T> poly;

        void compact();
        bool compare(const Polynomial &, const polynomial::Logical &) const;
        Polynomial operate(const Polynomial &, const polynomial::Operator &) const;
        Polynomial operate(const T &, const polynomial::Operator &) const;
        Polynomial & operate_inplace(const Polynomial &, const polynomial::Operator &);
        Polynomial & operate_inplace(const T &, const polynomial::Operator &);
        Polynomial rotate(int) const;
        Polynomial & rotate_inplace(int);

    public:
        Polynomial() {}
        Polynomial(const Polynomial & a) : Polynomial(a.poly) {}
        // Polynomial(const std::string &);
        Polynomial(const std::vector<T> &);

        explicit operator std::string() const;

        Polynomial & operator *=(const Polynomial &); // polynomial multiplication
        Polynomial & operator /=(const Polynomial &); // polynomial division
        Polynomial & operator %=(const Polynomial &); // polynomial reduction
        Polynomial & operator +=(const Polynomial &); // element-wise addition
        Polynomial & operator |=(const Polynomial &); // element-wise division
        Polynomial & operator &=(const Polynomial &); // element-wise multiplication
        Polynomial & operator -=(const Polynomial &); // element-wise subtraction
        // Polynomial & operator ^=(const Polynomial &); // undefined
        // Polynomial & operator <<=(const Polynomial &); // undefined
        // Polynomial & operator >>=(const Polynomial &); // undefined

        Polynomial & operator +=(const T &); // add scalar
        Polynomial & operator /=(const T &); // divide by scalar
        Polynomial & operator %=(const T &); // element modulo scalar
        Polynomial & operator *=(const T &); // multiply by scalar
        Polynomial & operator -=(const T &); // subtract scalar

        T & operator [](int); // access element
        Polynomial & operator<<=(int); // left rotation
        Polynomial & operator>>=(int); // right rotation

        Polynomial operator * (const Polynomial &) const;
        Polynomial operator / (const Polynomial &) const;
        Polynomial operator % (const Polynomial &) const;
        Polynomial operator + (const Polynomial &) const;
        Polynomial operator & (const Polynomial &) const;
        Polynomial operator | (const Polynomial &) const;
        Polynomial operator - (const Polynomial &) const;
        Polynomial operator ~ () const; // element-wise inversion

        Polynomial operator + (const T &) const;
        Polynomial operator / (const T &) const;
        Polynomial operator % (const T &) const;
        Polynomial operator * (const T &) const;
        Polynomial operator - (const T &) const;

        Polynomial operator<<(int);
        Polynomial operator>>(int);

        bool operator ==(const Polynomial &) const;
        bool operator >=(const Polynomial &) const;
        bool operator > (const Polynomial &) const;
        bool operator <=(const Polynomial &) const;
        bool operator < (const Polynomial &) const;
        bool operator !=(const Polynomial &) const;

        std::vector<T> get() const;
        int order() const;
        int size() const;

        template <class U>
        friend std::ostream & operator<<(std::ostream &, const Polynomial<U> &);
};

#include "polynomial.hpp" // implementation of template functions

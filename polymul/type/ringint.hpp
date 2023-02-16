#pragma once

#include <cstdlib>
#include <ostream>
#include <string>
#include <vector>

#include "number.h"
#include "integer.hpp"

namespace ringint
{
enum Operator { ADD, DIV, GCD, LCM, MOD, MUL, POW, SUB };
enum Logical { EQ, GE, GT, LE, LT, NE };
} // ringint

class Ringint
{
    private:
        Number x;
        static Number mod;
        const static int DEFAULT_BASE=10;
        // static bool isPrimitiveRoot, isRoot;
        // static std::vector<Number> primitiveRoots;
        // static std::vector<Number> roots, iroots;

        Ringint & operate(const Ringint &, const ringint::Operator &);
        Ringint operate_const(const Ringint &, const ringint::Operator &) const;
        bool operate_logical(const Ringint &, const ringint::Logical &) const;
        static Ringint operate_static(const Ringint &, const Ringint &, const ringint::Operator &);

    public:
        Ringint() : Ringint(0) {}
        Ringint(const Number &);
        Ringint(const Ringint & a) : Ringint(a.x) {}
        Ringint(const std::string & a) : Ringint(Number(a)) {}
        Ringint(uint64_t a) : Ringint(Number(a)) {}

        explicit operator bool() const;
        explicit operator uint64_t() const;
        explicit operator std::string() const;

        Ringint & operator +=(const Ringint &);
        Ringint & operator -=(const Ringint &);
        Ringint & operator *=(const Ringint &);
        Ringint & operator /=(const Ringint &);
        Ringint & operator %=(const Ringint &);
        Ringint & gcd(const Ringint &);
        Ringint & lcm(const Ringint &);
        Ringint & pow(const Ringint &);
        Ringint & inv();
        Ringint & operator++();
        Ringint & operator--();

        // Ringint & operator +=(uint64_t);
        // Ringint & operator -=(uint64_t);
        // Ringint & operator *=(uint64_t);
        // Ringint & operator /=(uint64_t);
        // Ringint & operator %=(uint64_t);

        Ringint operator++(int);
        Ringint operator--(int);
        Ringint operator +(const Ringint &) const;
        Ringint operator -(const Ringint &) const;
        Ringint operator *(const Ringint &) const;
        Ringint operator /(const Ringint &) const;
        Ringint operator %(const Ringint &) const;

        // Ringint operator +(uint64_t) const;
        // Ringint operator -(uint64_t) const;
        // Ringint operator *(uint64_t) const;
        // Ringint operator /(uint64_t) const;
        // Ringint operator %(uint64_t) const;

        bool operator !() const;
        bool operator==(const Ringint &) const;
        bool operator!=(const Ringint &) const;
        bool operator>=(const Ringint &) const;
        bool operator> (const Ringint &) const;
        bool operator<=(const Ringint &) const;
        bool operator< (const Ringint &) const;

        // bool operator==(uint64_t) const;
        // bool operator!=(uint64_t) const;
        // bool operator>=(uint64_t) const;
        // bool operator> (uint64_t) const;
        // bool operator<=(uint64_t) const;
        // bool operator< (uint64_t) const;

        static Ringint inv(const Ringint &);
        static Ringint gcd(const Ringint &, const Ringint &);
        static Ringint lcm(const Ringint &, const Ringint &);
        static Ringint pow(const Ringint &, const Ringint &);
        static Ringint random();

        static Number getModulus();
        // static Ringint primitiveRoot(int=1);
        static void setModulus(const Number &);

        std::string str(int base=DEFAULT_BASE) const;
        friend std::ostream & operator<<(std::ostream &, const Ringint &);
};

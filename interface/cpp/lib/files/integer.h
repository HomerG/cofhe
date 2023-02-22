#pragma once

#include <cstdint>
#include <gmp.h>
#include <ostream>
#include <string>

namespace integer
{
enum Operation { ADD, AND, DIV, GCD, INV, LCM, MOD, MUL, OR, POW, SLL, SLR, SUB, XOR };
enum Logical { EQ, GE, GT, LAN, LE, LOR, LT, NE };
} // integer

class Integer
{
    private:
        mpz_t x;
        const static int DEFAULT_BASE=10;
        const static int DEFAULT_REPETITIONS = 50; // recommended is [15,50]
        static bool isInitialized;
        static gmp_randstate_t state;
        static int primeLevel;

        static void init_random();
        Integer & operate(const Integer &, const integer::Operation &);
        Integer & operate(uint64_t, const integer::Operation &);
        Integer operate_const(const Integer &, const integer::Operation &) const;
        Integer operate_const(uint64_t, const integer::Operation &) const;
        bool operate_logical(const Integer &, const integer::Logical &) const;
        bool operate_logical(uint64_t, const integer::Logical &) const;
        static Integer operate_static(const Integer &, const Integer &, const integer::Operation &);

    public:
        Integer() : Integer( (uint64_t)(0) ) {}
        Integer(const char *, int base=DEFAULT_BASE);
        Integer(const Integer & a) : Integer(a.x) {}
        Integer(const mpz_t &);
        Integer(const std::string & a, int base=DEFAULT_BASE) : Integer(a.c_str(), base) {}
        Integer(int a)               : Integer(uint64_t(a)) {}
        Integer(unsigned a)          : Integer(uint64_t(a)) {}
        Integer(unsigned long long a): Integer(uint64_t(a)) {}
        Integer(long long a)         : Integer(uint64_t(a)) {}
        Integer(int64_t a)           : Integer(uint64_t(a)) {}
        Integer(uint64_t);
        ~Integer();

        explicit operator bool() const;
        explicit operator uint64_t() const;
        explicit operator std::string() const;

        Integer & operator  =(const Integer &);
        Integer & operator +=(const Integer &);
        Integer & operator -=(const Integer &);
        Integer & operator *=(const Integer &);
        Integer & operator /=(const Integer &);
        Integer & operator %=(const Integer &);
        Integer & operator<<=(const Integer &);
        Integer & operator>>=(const Integer &);
        Integer & operator &=(const Integer &);
        Integer & operator |=(const Integer &);
        Integer & operator ^=(const Integer &);
        Integer & gcd(const Integer &);
        Integer & inv(const Integer &);
        Integer & lcm(const Integer &);
        Integer & pow(const Integer &);
        Integer & powm(const Integer &, const Integer &);

        Integer & operator++();
        Integer & operator--();
        Integer & operator +=(uint64_t);
        Integer & operator -=(uint64_t);
        Integer & operator *=(uint64_t);
        Integer & operator /=(uint64_t);
        Integer & operator %=(uint64_t);
        Integer & operator<<=(uint64_t);
        Integer & operator>>=(uint64_t);
        Integer & operator &=(uint64_t);
        Integer & operator |=(uint64_t);
        Integer & operator ^=(uint64_t);
        Integer & pow(uint64_t);
        Integer & powm(uint64_t, const Integer &);

        Integer operator +(const Integer &) const;
        Integer operator -(const Integer &) const;
        Integer operator *(const Integer &) const;
        Integer operator /(const Integer &) const;
        Integer operator %(const Integer &) const;
        Integer operator<<(const Integer &) const;
        Integer operator>>(const Integer &) const;
        Integer operator &(const Integer &) const;
        Integer operator |(const Integer &) const;
        Integer operator ^(const Integer &) const;

        Integer operator++(int);
        Integer operator--(int);
        Integer operator +(uint64_t) const;
        Integer operator -(uint64_t) const;
        Integer operator *(uint64_t) const;
        Integer operator /(uint64_t) const;
        Integer operator %(uint64_t) const;
        Integer operator<<(uint64_t) const;
        Integer operator>>(uint64_t) const;
        Integer operator &(uint64_t) const;
        Integer operator |(uint64_t) const;
        Integer operator ^(uint64_t) const;

        bool operator[](int) const;
        bool operator !() const;
        bool operator&&(const Integer &) const;
        bool operator||(const Integer &) const;
        bool operator==(const Integer &) const;
        bool operator!=(const Integer &) const;
        bool operator>=(const Integer &) const;
        bool operator> (const Integer &) const;
        bool operator<=(const Integer &) const;
        bool operator< (const Integer &) const;

        bool operator&&(uint64_t) const;
        bool operator||(uint64_t) const;
        bool operator==(uint64_t) const;
        bool operator!=(uint64_t) const;
        bool operator>=(uint64_t) const;
        bool operator> (uint64_t) const;
        bool operator<=(uint64_t) const;
        bool operator< (uint64_t) const;
        bool isPrime() const;

        static Integer gcd(const Integer &, const Integer &);
        static Integer inv(const Integer &, const Integer &);
        static Integer lcm(const Integer &, const Integer &);
        static Integer nextPrime(const Integer &);
        static Integer pow(const Integer &, const Integer &);
        static Integer pow(const Integer &, uint64_t);
        static Integer powm(const Integer &, const Integer &, const Integer &);
        static Integer powm(const Integer &, uint64_t, const Integer &);
        static Integer random(const Integer &);
        static Integer random(uint64_t);
        static Integer randomPrime(const Integer &);
        static Integer randomPrime(uint64_t);
        static bool isPrime(const Integer &);
        static void setPrimeLevel(int primeLevel);

        std::string str(int base=DEFAULT_BASE) const;
        friend std::ostream & operator<<(std::ostream &, const Integer &);

        friend Integer operator +(uint64_t, const Integer &);
        friend Integer operator -(uint64_t, const Integer &);
        friend Integer operator *(uint64_t, const Integer &);
        friend Integer operator /(uint64_t, const Integer &);
        friend Integer operator %(uint64_t, const Integer &);
        friend Integer operator<<(uint64_t, const Integer &);
        friend Integer operator>>(uint64_t, const Integer &);
        friend Integer operator &(uint64_t, const Integer &);
        friend Integer operator |(uint64_t, const Integer &);
        friend Integer operator ^(uint64_t, const Integer &);

        friend bool operator &&(uint64_t, const Integer &);
        friend bool operator ||(uint64_t, const Integer &);
        friend bool operator ==(uint64_t, const Integer &);
        friend bool operator !=(uint64_t, const Integer &);
        friend bool operator >=(uint64_t, const Integer &);
        friend bool operator > (uint64_t, const Integer &);
        friend bool operator <=(uint64_t, const Integer &);
        friend bool operator < (uint64_t, const Integer &);
};

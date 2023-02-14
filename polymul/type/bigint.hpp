#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include "e3int.h"

class Bigint
{
    private:
        e3::Bigun x;
        const static int DEFAULT_BASE=10;

    public:
        Bigint() : Bigint( (uint64_t)(0) ) {}
        Bigint(const char * a, int base=DEFAULT_BASE) : Bigint( e3::Bigun(a) ) {}
        Bigint(const Bigint & a) : Bigint(a.x) {}
        Bigint(const e3::Bigun & a) { x = a; }
        Bigint(const std::string & a, int base=DEFAULT_BASE) : Bigint(a.c_str(), base) {}
        Bigint(int a): Bigint(uint64_t(a)) {}
        Bigint(uint64_t a) : Bigint( e3::Bigun( (unsigned long long)(a) ) ) {}

        explicit operator bool() const;
        explicit operator uint64_t() const;
        explicit operator std::string() const;

        Bigint & operator +=(const Bigint &);
        Bigint & operator -=(const Bigint &);
        Bigint & operator *=(const Bigint &);
        Bigint & operator /=(const Bigint &);
        Bigint & operator %=(const Bigint &);
        Bigint & operator<<=(const Bigint &);
        Bigint & operator>>=(const Bigint &);
        Bigint & operator &=(const Bigint &);
        Bigint & operator |=(const Bigint &);
        Bigint & operator ^=(const Bigint &);
        Bigint & gcd(const Bigint &);
        Bigint & inv(const Bigint &);
        Bigint & lcm(const Bigint &);
        Bigint & pow(const Bigint &, const Bigint &);

        Bigint & operator++();
        Bigint & operator--();
        // Bigint & operator +=(uint64_t);
        // Bigint & operator -=(uint64_t);
        // Bigint & operator *=(uint64_t);
        // Bigint & operator /=(uint64_t);
        // Bigint & operator %=(uint64_t);
        // Bigint & operator<<=(uint64_t);
        // Bigint & operator>>=(uint64_t);
        // Bigint & operator &=(uint64_t);
        // Bigint & operator |=(uint64_t);
        // Bigint & operator ^=(uint64_t);
        // Bigint & pow(uint64_t, const Bigint &);

        Bigint operator +(const Bigint &) const;
        Bigint operator -(const Bigint &) const;
        Bigint operator *(const Bigint &) const;
        Bigint operator /(const Bigint &) const;
        Bigint operator %(const Bigint &) const;
        Bigint operator<<(const Bigint &) const;
        Bigint operator>>(const Bigint &) const;
        Bigint operator &(const Bigint &) const;
        Bigint operator |(const Bigint &) const;
        Bigint operator ^(const Bigint &) const;

        Bigint operator++(int);
        Bigint operator--(int);
        // Bigint operator +(uint64_t) const;
        // Bigint operator -(uint64_t) const;
        // Bigint operator *(uint64_t) const;
        // Bigint operator /(uint64_t) const;
        // Bigint operator %(uint64_t) const;
        // Bigint operator<<(uint64_t) const;
        // Bigint operator>>(uint64_t) const;
        // Bigint operator &(uint64_t) const;
        // Bigint operator |(uint64_t) const;
        // Bigint operator ^(uint64_t) const;

        bool operator[](int) const;
        bool operator !() const;
        bool operator&&(const Bigint &) const;
        bool operator||(const Bigint &) const;
        bool operator==(const Bigint &) const;
        bool operator!=(const Bigint &) const;
        bool operator>=(const Bigint &) const;
        bool operator> (const Bigint &) const;
        bool operator<=(const Bigint &) const;
        bool operator< (const Bigint &) const;

        // bool operator&&(uint64_t) const;
        // bool operator||(uint64_t) const;
        // bool operator==(uint64_t) const;
        // bool operator!=(uint64_t) const;
        // bool operator>=(uint64_t) const;
        // bool operator> (uint64_t) const;
        // bool operator<=(uint64_t) const;
        // bool operator< (uint64_t) const;

        static Bigint gcd(const Bigint &, const Bigint &);
        static Bigint inv(const Bigint &, const Bigint &);
        static Bigint lcm(const Bigint &, const Bigint &);
        static Bigint pow(const Bigint &, const Bigint &, const Bigint &);
        // static Bigint pow(const Bigint &, uint64_t, const Bigint &);

        std::string str(int base=DEFAULT_BASE) const;
        friend std::ostream & operator<<(std::ostream &, const Bigint &);

        friend Bigint operator +(uint64_t, const Bigint &);
        friend Bigint operator -(uint64_t, const Bigint &);
        friend Bigint operator *(uint64_t, const Bigint &);
        friend Bigint operator /(uint64_t, const Bigint &);
        friend Bigint operator %(uint64_t, const Bigint &);
        friend Bigint operator<<(uint64_t, const Bigint &);
        friend Bigint operator>>(uint64_t, const Bigint &);
        friend Bigint operator &(uint64_t, const Bigint &);
        friend Bigint operator |(uint64_t, const Bigint &);
        friend Bigint operator ^(uint64_t, const Bigint &);

        friend bool operator &&(uint64_t, const Bigint &);
        friend bool operator ||(uint64_t, const Bigint &);
        friend bool operator ==(uint64_t, const Bigint &);
        friend bool operator !=(uint64_t, const Bigint &);
        friend bool operator >=(uint64_t, const Bigint &);
        friend bool operator > (uint64_t, const Bigint &);
        friend bool operator <=(uint64_t, const Bigint &);
        friend bool operator < (uint64_t, const Bigint &);
};

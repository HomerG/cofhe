#pragma once

#include <complex>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

// TFHE=0 SEAL=0 FHEW=0 HELI=0 MPIR=1 CUDD=0 PALI=0

// including MPIR
//#include "mpir.h"
//#include "mpirxx.h"
//#include "gmp-impl.h"
#include <gmpxx.h>
// start header amalgama

// *** begin cpp_share/util/e3util.h
#ifndef _E3_E3UTIL_H_
#define _E3_E3UTIL_H_


namespace e3
{
namespace util
{

using ull = unsigned long long;
using sll = signed long long;
using usi = unsigned short int;

ull hex2ull(std::string);
std::string ull2hex(ull);

constexpr inline bool bitAt(ull m, ull n) { return 0 != (m & (1ull << n)); }

constexpr inline ull mask(ull n)
{
    if ( n >= 8 * sizeof(1ull) ) return -1;
    return (1ull << n) - 1;
}

constexpr inline sll signExtend(sll m, ull n)
{
    if (!bitAt(m, n - 1)) return m;
    return m | ~mask(n);
}

inline bool isHex(char c) { return ( c >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ); }
bool isHex(const std::string & hex);

std::string bin2hex(const std::vector<bool> & bin);
std::vector<bool> hex2bin(const std::string & hex, size_t bitsize = 0);

std::string bin2sbn(const std::vector<bool> & bin);
std::vector<bool> sbn2bin(const std::string & sbn, size_t bitsize = 0);

unsigned hex2dec(char hex);
char hex2dec(char hex, bool * carry);
char dec2hex(unsigned dec);
std::string hex2dec(const std::string & hex, size_t bitsize = 0, bool sign = false);
std::string dec2hex(const std::string & dec, size_t bitsize);

bool isNegative(const std::string & hex, size_t bitsize);
std::string twosComplement(const std::string & hex, size_t bitsize);
std::string increment(const std::string & hex, size_t bitsize);
char negation(char hex);
std::string negation(const std::string & hex, size_t bitsize);
std::string add(const std::string & dec, unsigned value);
std::string multiply(const std::string & dec, unsigned value);
std::string removeLeadingZeros(const std::string & s);
char addHex(char hex1, char hex2, bool * carry = nullptr);
std::string divide(const std::string & dec, unsigned div);
unsigned mod(const std::string & dec, unsigned div);
char maskHex(char c, size_t bitsize);
std::string zeroExtension(const std::string & hex, size_t bitsize);

std::vector<std::string> split(const std::string & s, char c);

inline std::string to_string(const std::string & s) { return s; }
template <class T> inline std::string to_string(const std::complex<T> & s) { return std::to_string(s.real()) + "+" + std::to_string(s.imag()) + "i"; }
template <class T> inline std::string to_string(const T & s) { return std::to_string(s); }

template <class T>
inline std::string merge(const std::vector<T> & v, std::string bind)
{
    std::string s = "";
    if ( !v.empty() )
    {
        s += to_string(v[0]);
        for ( size_t i = 1; i < v.size(); i++ )
            s += bind + to_string(v[i]);
    }
    return s;
}

template <class T>
inline std::vector<T> extend(const std::vector<T> & v, size_t size, const T & value)
{
    std::vector<T> r;
    r.assign(v.begin(), v.end());
    r.resize(size, value);
    return r;
}

std::string trim(const std::string & s);
std::string loadConst(std::string typ, std::string id);

inline size_t msb(size_t n)
{
    size_t msb = 0;
    for (; n; n >>= 1 ) msb++;
    return msb;
}

template <class T>
inline T sum_inplace(std::vector<T> & v)
{
    auto size = v.size();
    for ( auto n = 1 << msb(size); n > 1; )
    {
        n >>= 1;
        for ( size_t i = n; i < size; i++ ) v[i - n] += v[i];
        size = n;
    }
    v.resize(1);
    return v[0];
}

template <class T>
inline T sum(const std::vector<T> & v)
{
    std::vector<T> vtmp;
    auto half = v.size() >> 1;
    for ( size_t i = 0; i < half; i++ ) vtmp.push_back( v[i] + v[i + half] );
    if ( v.size() & 1 ) vtmp.push_back( v.back() ); // if v.size is odd
    return sum_inplace(vtmp);
}

template <class T>
inline T product_inplace(std::vector<T> & v)
{
    auto size = v.size();
    for ( auto n = 1 << msb(size); n > 1; )
    {
        n >>= 1;
        for ( size_t i = n; i < size; i++ ) v[i - n] *= v[i];
        size = n;
    }
    v.resize(1);
    return v[0];
}

template <class T>
inline T product(const std::vector<T> & v)
{
    std::vector<T> vtmp;
    auto half = v.size() >> 1;
    for ( size_t i = 0; i < half; i++ ) vtmp.push_back( v[i] * v[i + half] );
    if ( v.size() & 1 ) vtmp.push_back( v.back() ); // if v.size is odd
    return product_inplace(vtmp);
}

} // util
} // e3


// using these macro during active development phase
#ifdef DEVMACROS
#define HALT throw std::string("\nFIX: ")+(__FILE__)+":"+std::to_string(__LINE__);
#define WARN std::printf("FIX: %s:%d\n", __FILE__, __LINE__);
#define MSGE(x) std::printf("FIX: %s\n", x);
#endif

#ifndef NOVERCHK
// === testing compiler versions
// test MS VERSION
#ifdef _MSC_VER
#if _MSC_VER < 1910
#error MS compiler version must be not less than 19.16
#endif
#endif
// test GCC version
#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION < 50400
#error GCC compiler version must be not less than 5.4.0
#endif
#endif
// === END testing compiler versions
#endif // NOVERCHK

#endif // _E3_E3UTIL_H_
// *** end cpp_share/util/e3util.h

// *** begin cpp_share/util/e3math.h
#ifndef _E3_E3MATH_H_
#define _E3_E3MATH_H_


namespace e3
{
namespace math
{

template<typename T> bool invert(const T & x, const T & mod, T * r);

template<typename Unumber>
struct UnumberContractor
{
    using UC = UnumberContractor;

    Unumber x;
    UnumberContractor() {}
    UnumberContractor(const Unumber & a) : x(a) {}

    // Constrains
    bool operator==(const UC & a) const { return x == a.x; }
    bool operator<(const UC & a) const { return x < a.x; }
    void add(const UC & b) { x += b.x; }
    void sub(const UC & b) { x -= b.x; }
    std::string str() const { return x.str(); }

    // Derived
    bool operator!=(const UC & a) const { return !(*this == a); }
    void subMod(const UC & b, const UC & mod) { if ( *this < b) add(mod); sub(b); }
    void addMod(const UC & b, const UC & mod)
    {
        add(b);
            while (1) if ( *this < mod ) break; else sub(mod);
    }
    bool operator>(const UC & a) const { return !(*this < a || *this == a); }

    void swap(UC & a) { x.swap(a.x); }
    void mulMod(const UC & b, const UC & mod) { x = x.mul(b.x, mod.x); }
    static void divABRQ(const UC & a, const UC & b, UC * r, UC * q)
    { a.x.divABRQ(b.x, r ? &r->x : 0, q ? &q->x : 0); }
};

template<>
struct UnumberContractor<e3::util::ull>
{
    using UC = UnumberContractor;
    using Unumber = e3::util::ull;

    Unumber x;
    UnumberContractor(): x() {}
    UnumberContractor(const Unumber & a) : x(a) {}

    // Constrains
    bool operator==(const UC & a) const { return x == a.x; }
    bool operator<(const UC & a) const { return x < a.x; }
    void add(const UC & b) { x += b.x; }
    void sub(const UC & b) { x -= b.x; }
    std::string str() const { return std::to_string(x); }

    // Derived
    bool operator!=(const UC & a) const { return !(*this == a); }
    void subMod(const UC & b, const UC & mod) { if ( *this < b) add(mod); sub(b); }
    void addMod(const UC & b, const UC & mod)
    {
        add(b);
            while (1) if ( *this < mod ) break; else sub(mod);
    }
    bool operator>(const UC & a) const { return !(*this < a || *this == a); }

    void swap(UC & a) { std::swap(x, a.x); }
    void mulMod(const UC & b, const UC & mod) { x = (x * b.x % mod.x); }
    static void divABRQ(const UC & a, const UC & b, UC * r, UC * q)
    { if (r) r->x = a.x % b.x; if (q) q->x = a.x / b.x; }
};

template <class T>
class Meuclid
{
    public:

        struct Rec
        {
            T a, b, c, d, e, f, g;
        };

    private:

        T et, ft;
        typedef std::vector<Rec> Vr;
        Vr v;
        T gsmall;

        T mod;

        void fillAB(T x, T y);
        void fillEF();
        void fillG();

        void checkRec(const Rec & v0, const Rec & v,
                      const Rec & vb, T et, T ft);

        T mul(const T & a, const T & b) const { T t(a); t.mulMod(b, mod); return t; }
        T sub(const T & a, const T & b) const { T t(a); t.subMod(b, mod); return t; }
        T add(const T & a, const T & b) const { T t(a); t.addMod(b, mod); return t; }
        T neg(const T & a) const { T t(0); t.subMod(a, mod); return t; }
        T divq(const T & a, const T & b) const { T q; T::divABRQ(a, b, 0, &q); return q; }

    public:
        Meuclid(T x, T y, T m): et(0), ft(1), gsmall(0), mod(m) { fillAB(x, y); fillEFG(); }

        void fillEFG() { fillEF(); fillG(); }
        void setEF(T e, T f) { et = e; ft = f; }

        T gcd() const { return v[v.size() - 1].b; }

        std::string str() const;
        int size() const { return int(v.size()); }

        const Rec & operator[](int i) const { return v[i]; }

        T smallg() const { return gsmall; }

};


template <class T>
class Invertor
{
        Meuclid<T> e;
    public:
        Invertor(T x, T m): e(x, m, m) {}

        bool isOk() const { return e[e.size() - 1].b == T(1); }
        T get() const { return e[e.size() - 1].f; }
};



} // math
} // e3

#endif // _E3_E3MATH_H_
// *** end cpp_share/util/e3math.h

// *** begin cpp_share/util/base64.h
#ifndef _E3_BASE64_H_
#define _E3_BASE64_H_


namespace e3
{
namespace util
{
namespace base64
{

std::string encode(unsigned char const * data, unsigned int data_length);
std::string decode(std::string const & data);

template <class T, class U>
inline const T * cp2cp(const U * p) { return reinterpret_cast<const T *>( p ); }
inline int st2i(size_t x) { return static_cast<int>( x ); }

inline std::string enc(const std::string & s)
{
    return encode(cp2cp<unsigned char, char>(s.c_str()), st2i(s.size()));
}

inline std::string dec(const std::string & s) { return decode(s); }

} // base64
} // util
} // e3

#endif // _E3_BASE64_H_
// *** end cpp_share/util/base64.h

// *** begin cpp_share/util/cophee.h

using std::uint32_t;

namespace e3
{

namespace cophee
{

class Cophee
{
    private:
        static int handler;
        static int handlerArduino;
        static int baudRate;
        static bool inited;
        static bool isUsingArduino;
        static bool isParamsSet;

        void init();
        void init_arduino();
        void init_chip();
        void init_port();
        void init_port_baud(uint32_t baud);

        uint32_t read_serial (uint32_t address) const;
        std::vector<uint32_t> read_serial_n  (uint32_t address, size_t size) const;
        std::vector<uint32_t> read_serial_1x (uint32_t address) const;
        std::vector<uint32_t> read_serial_2x (uint32_t address) const;

        void write_serial (uint32_t address, uint32_t data) const;
        void write_serial_n  (uint32_t address, const std::vector<uint32_t> & data, size_t size) const;
        void write_serial_1x (uint32_t address, const std::vector<uint32_t> & data) const;
        void write_serial_2x (uint32_t address, const std::vector<uint32_t> & data) const;

        void wait_for_interrupt() const;

    public:
        Cophee() : Cophee(true) {}
        Cophee(int baudRate) : Cophee(baudRate, false) {}
        Cophee(int baudRate, bool isUsingArduino);
        Cophee(const Cophee & a) : Cophee(a.baudRate, a.isUsingArduino) {}

        bool is_params_set() const;
        void set_params(const std::vector<uint32_t> & n, const std::vector<uint32_t> & n2,
                        const std::vector<uint32_t> & fkf) const;
        void set_params(const std::vector<uint32_t> & n, const std::vector<uint32_t> & n2,
                        const std::vector<uint32_t> & rand0, const std::vector<uint32_t> & rand1,
                        const std::vector<uint32_t> & fkf) const;
        std::vector<uint32_t> mod_mul (const std::vector<uint32_t> & a, const std::vector<uint32_t> & b) const;
        std::vector<uint32_t> mod_exp (const std::vector<uint32_t> & a, const std::vector<uint32_t> & b) const;
        std::vector<uint32_t> mod_inv (const std::vector<uint32_t> & a, const std::vector<uint32_t> & b) const;
        std::vector<uint32_t> gfunc   (const std::vector<uint32_t> & a, const std::vector<uint32_t> & b) const;
};

} // cophee

} // e3
// *** end cpp_share/util/cophee.h

// *** begin cpp_share/util/def_fhew.h
#ifndef _E3_DEF_FHEW_H_
#define _E3_DEF_FHEW_H_


namespace e3
{

struct LweCipherText;

using FhewNativePrivKey = void * ;
using FhewNativeEvalKey = void * ;
using cFhewNativeEvalKey = const void * ;

struct FhewNativeBt
{
    std::shared_ptr<LweCipherText> p;
    std::string str(cFhewNativeEvalKey) const;
    FhewNativeBt(cFhewNativeEvalKey ek); // new
    FhewNativeBt(const std::string & enc, cFhewNativeEvalKey ek);
    FhewNativeBt(const FhewNativeBt &) = delete;
    FhewNativeBt(const FhewNativeBt & b, cFhewNativeEvalKey ek);
};

int fhew_impl(); // 0 or 1

} // e3


#endif // _E3_DEF_FHEW_H_

// *** end cpp_share/util/def_fhew.h

// *** begin cpp_share/util/def_fhew0.h
#ifndef _E3_DEF_FHEW0_H_
#define _E3_DEF_FHEW0_H_

// this header def_fhew0.h should not be included in any code except *fhew0.cpp


namespace e3
{

struct LweCipherText { bool b; };

} // e3

#endif // _E3_DEF_FHEW0_H_
// *** end cpp_share/util/def_fhew0.h

// *** begin cpp_share/util/def_heli.h
#ifndef _E3_DEF_HELI_H_
#define _E3_DEF_HELI_H_


namespace e3
{

struct HeliCtxt;

using HeliNativePrivKey = void * ;
using HeliNativeEvalKey = void * ;
using cHeliNativeEvalKey = const void * ;

struct NativeProperties;

struct HeliNativeBt
{
    cHeliNativeEvalKey ek;
    std::shared_ptr<HeliCtxt> ctxt;
    std::string str(cHeliNativeEvalKey) const;
    HeliNativeBt(cHeliNativeEvalKey ek); // new
    HeliNativeBt(const std::string & enc, cHeliNativeEvalKey ek);
    HeliNativeBt(const HeliNativeBt &) = delete;
    HeliNativeBt(const HeliNativeBt & b, cHeliNativeEvalKey ek);
};

int heli_impl(); // 0 or 1

} // e3

#endif // _E3_DEF_HELI_H_

// *** end cpp_share/util/def_heli.h

// *** begin cpp_share/util/def_heli0.h
#ifndef _E3_DEF_HELI0_H_
#define _E3_DEF_HELI0_H_

// this header def_heli0.h should not be included in any code except *heli0.cpp


namespace e3
{

struct HeliCtxt { bool b; };

struct NativeProperties {};

} // e3

#endif // _E3_DEF_HELI0_H_
// *** end cpp_share/util/def_heli0.h

// *** begin cpp_share/util/def_mpir.h
#ifndef _E3_DEF_MPIR_H_
#define _E3_DEF_MPIR_H_


using std::string;

namespace e3
{

int mpir_impl(); // 0 or 1

struct BigunNative;

class Bigun;
std::istream & operator>>(std::istream & is, Bigun & x);
std::ostream & operator<<(std::ostream & os, const Bigun & x);

class Bigun
{
        std::shared_ptr<BigunNative> p;

    protected:

        using ull_t = unsigned long long;

    public:
        explicit Bigun( ull_t = 0 );
        Bigun(string x);

        Bigun(const Bigun & a);
        Bigun & operator=(const Bigun & a);

        Bigun(Bigun && a) noexcept;
        Bigun & operator=(Bigun && a) noexcept;

        static int maxbitsz();

        string str() const;
        string sth() const; // hex

        Bigun powmod(Bigun x, Bigun m) const;
        Bigun addmod(Bigun x, Bigun m) const;
        Bigun submod(Bigun x, Bigun m) const;
        Bigun mulmod(Bigun x, Bigun m) const;
        Bigun invmod(Bigun m) const;

        Bigun powmod(unsigned x, Bigun m) const { return powmod(Bigun(x),m); }

        bool isZero() const;
        explicit operator bool() { return !isZero(); }

        friend std::istream & operator>>(std::istream & is, Bigun & x);
        friend std::ostream & operator<<(std::ostream & os, const Bigun & x);

        Bigun & operator+=(const Bigun & a);
        Bigun & operator-=(const Bigun & a);
        Bigun & operator*=(const Bigun & a);
        Bigun & operator/=(const Bigun & a);
        Bigun & operator%=(const Bigun & a);
        Bigun & operator&=(const Bigun & a);
        Bigun & operator|=(const Bigun & a);
        Bigun & operator^=(const Bigun & a);
        Bigun & operator<<=(const Bigun & a);
        Bigun & operator>>=(const Bigun & a);

        Bigun & operator|=(int a) { return *this |= Bigun(ull_t(a)); }
        Bigun & operator<<=(int a) { return *this <<= Bigun(ull_t(a)); }
        Bigun & operator>>=(int a) { return *this >>= Bigun(ull_t(a)); }
        Bigun operator<<(int a) const { Bigun r(*this); return r <<= a; }
        Bigun operator>>(int a) const { Bigun r(*this); return r >>= a; }

        Bigun & operator++();
        Bigun & operator--();
        Bigun operator++(int) { Bigun x(*this); ++*this; return x; }
        Bigun operator--(int) { Bigun x(*this); --*this; return x; }
        Bigun operator-() const;
        Bigun operator+() const { return *this; }

        Bigun operator+(const Bigun & a) const { Bigun r(*this); return r += a; }
        Bigun operator-(const Bigun & a) const { Bigun r(*this); return r -= a; }
        Bigun operator*(const Bigun & a) const { Bigun r(*this); return r *= a; }
        Bigun operator/(const Bigun & a) const { Bigun r(*this); return r /= a; }
        Bigun operator%(const Bigun & a) const { Bigun r(*this); return r %= a; }
        Bigun operator&(const Bigun & a) const { Bigun r(*this); return r &= a; }
        Bigun operator|(const Bigun & a) const { Bigun r(*this); return r |= a; }
        Bigun operator^(const Bigun & a) const { Bigun r(*this); return r ^= a; }
        Bigun operator<<(const Bigun & a) const { Bigun r(*this); return r <<= a; }
        Bigun operator>>(const Bigun & a) const { Bigun r(*this); return r >>= a; }
        Bigun operator~() const;
        bool operator!() const { return isZero(); }
        bool operator==(const Bigun & a) const;
        bool operator!=(const Bigun & a) const { return !(*this == a); }
        bool operator<(const Bigun & a) const;
        bool operator>=(const Bigun & a) const { return !(*this < a); }
        bool operator>(const Bigun & a) const { return a < *this; }
        bool operator<=(const Bigun & a) const { return !(a < *this); }
        bool operator&&(const Bigun & a) const { return !!*this && !!a; }
        bool operator||(const Bigun & a) const { return !!*this || !!a; }

        friend Bigun operator+(ull_t a, const Bigun & b) { return b + Bigun(a); }
        friend Bigun operator-(ull_t a, const Bigun & b) { return b - Bigun(a); }
        friend Bigun operator*(ull_t a, const Bigun & b) { return b * Bigun(a); }
        friend Bigun operator+(const Bigun & b, ull_t a) { return b + Bigun(a); }
        friend Bigun operator-(const Bigun & b, ull_t a) { return b - Bigun(a); }
        friend Bigun operator*(const Bigun & b, ull_t a) { return b * Bigun(a); }
        friend Bigun operator/(const Bigun & b, ull_t a) { return b / Bigun(a); }
        friend Bigun operator%(const Bigun & b, ull_t a) { return b % Bigun(a); }

        ull_t ull() const;

        // bit access
        ///struct BitVal
        ///{
        ///    Bigun v; // 0 or 1
        ///    bool operator!() const { return !v; }
        ///};
        ///using BitVal = Bigun;
        struct BitVal
        {
            bool v; // 0 or 1
            bool operator!() const { return !v; }
        };

        struct BitRef
        {
            Bigun * p = nullptr;
            int i;

            BitVal val() const;
            void setbit(BitVal b);

            BitRef operator=(BitVal b) { setbit(b); return *this; }
            BitRef operator=(int x) { setbit(BitVal {!!x}); return *this; }
            BitRef operator=(const BitRef & b) { setbit(b.val()); return *this; }

            bool operator!() const { return !val(); }
        };
        BitRef operator[](int i) { return BitRef {this, i}; }
        BitVal operator()(int i) const { Bigun t(*this); return t[i].val(); }
        friend Bigun operator*(BitVal a, const Bigun & b) { return (!a)?Bigun(0):b; }
        friend Bigun operator*(const BitRef &a, const Bigun & b) { return a.val() * b; }


        // CoPHEE
        std::vector<uint32_t> data() const;
        void data(const std::vector<uint32_t> &);
};


inline std::ostream & operator<<(std::ostream & os, const Bigun & x) { return os << x.str(); }

} // e3

#endif // _E3_DEF_MPIR_H_
// *** end cpp_share/util/def_mpir.h

// *** begin cpp_share/util/def_mpir1.h
#ifndef _E3_DEF_MPIR1_H_
#define _E3_DEF_MPIR1_H_

// this header def_mpir1.h should not be included in any code except *mpir1.cpp

// MPIR headers

// local headers

namespace e3
{

struct BigunNative
{
    mpz_class n;
    BigunNative( unsigned long long x = 0): n((unsigned long)x) {}
    BigunNative( mpz_class a ): n(a) {}

    string str() const;
    BigunNative powmod(BigunNative x, BigunNative m) const;
    BigunNative mulmod(BigunNative x, BigunNative m) const;
    BigunNative invmod(BigunNative m) const;

    bool isZero() const;

    BigunNative & operator+=(const BigunNative & a) { n += a.n; return *this; }
    BigunNative & operator-=(const BigunNative & a) { n -= a.n; return *this; }
    BigunNative & operator*=(const BigunNative & a) { n *= a.n; return *this; }
    BigunNative & operator/=(const BigunNative & a) { n /= a.n; return *this; }
    BigunNative & operator%=(const BigunNative & a) { n %= a.n; return *this; }
    BigunNative & operator&=(const BigunNative & a) { n &= a.n; return *this; }
    BigunNative & operator|=(const BigunNative & a) { n |= a.n; return *this; }
    BigunNative & operator^=(const BigunNative & a) { n ^= a.n; return *this; }
    BigunNative & operator<<=(const BigunNative & a);
    BigunNative & operator>>=(const BigunNative & a);

    BigunNative & operator++() { ++n; return *this; }
    BigunNative & operator--() { --n; return *this; }
    BigunNative operator~() const { return BigunNative(~n); }
    BigunNative operator-() const { return BigunNative(-n); }

    bool operator==(const BigunNative & a) const { return n == a.n; }
    bool operator<(const BigunNative & a) const { return n < a.n; }

    unsigned long long ull() const { return n.get_ui(); }

    // CoPHEE
    std::vector<uint32_t> data(size_t nitems = 64) const;
    void data(const std::vector<uint32_t> &);
};

} // e3

#endif // _E3_DEF_MPIR1_H_
// *** end cpp_share/util/def_mpir1.h

// *** begin cpp_share/util/def_pil.h
#ifndef _E3_DEF_PIL_H_
#define _E3_DEF_PIL_H_



namespace e3
{

// operations are defined at eval key header (ek_circ_pil)
struct PilNum
{
    Bigun n;
    PilNum() {}
    explicit PilNum(Bigun a): n(a) {}
    explicit PilNum(unsigned long long a): n(a) {}
};

struct PilPair
{
    PilNum a, b;
    PilPair() {}
    PilPair(PilNum qa, PilNum qb): a(qa), b(qb) {}
};

struct PilQuad
{
    PilPair x, y;
    PilQuad() {}
    PilQuad(PilPair qa, PilPair qb): x(qa), y(qb) {}
    explicit PilQuad(string s);
    string str() const;
};

} // e3


#endif // _E3_DEF_PIL_H_
// *** end cpp_share/util/def_pil.h

// *** begin cpp_share/util/def_seal.h
#ifndef _E3_DEF_SEAL_H_
#define _E3_DEF_SEAL_H_



namespace e3
{

struct SealCiphertext;

using SealNativePrivKey = void * ;
using SealNativeEvalKey = void * ;
using cSealNativeEvalKey = const void * ;

struct SealNativeCiphertext
{
    std::shared_ptr<SealCiphertext> p;
    std::string str() const;

    SealNativeCiphertext(); // new
    SealNativeCiphertext(const SealNativeCiphertext &); // = delete;
    SealNativeCiphertext(const std::string & enc, cSealNativeEvalKey ek);
};

int seal_impl(); // 0 or 1

} // e3

#endif // _E3_DEF_SEAL_H_
// *** end cpp_share/util/def_seal.h

// *** begin cpp_share/util/def_seal0.h
#ifndef _E3_DEF_SEAL0_H_
#define _E3_DEF_SEAL0_H_

// this header def_seal0.h should not be included in any code except *seal0.cpp


namespace e3
{

struct SealCiphertext { std::vector<uint64_t> ct = {0}; };

namespace e3seal
{
// struct SealPrivKey
// {
// };

struct SealEvalKey
{
    uint64_t plaintextModulus = 0;
    uint64_t polyModulusDegree = 0;
    bool isBatchEncoder = false;
};

// using sk = SealPrivKey;
// inline  sk * tosk(e3::SealNativePrivKey a) { return static_cast<sk *>(a); }

using ek = SealEvalKey;
inline  ek * toek(e3::SealNativeEvalKey a) { return static_cast<ek *>(a); }
inline  const ek * toek(e3::cSealNativeEvalKey a) { return static_cast<const ek *>(a); }
} //e3seal
} // e3
#endif // _E3_DEF_SEAL0_H_
// *** end cpp_share/util/def_seal0.h

// *** begin cpp_share/util/def_seal_ckks.h
#ifndef _E3_DEF_SEAL_CKKS_H_
#define _E3_DEF_SEAL_CKKS_H_



namespace e3
{
struct SealCkksCiphertext;

using SealCkksNativePrivKey = void * ;
using SealCkksNativeEvalKey = void * ;
using cSealCkksNativeEvalKey = const void * ;

struct SealCkksNativeCiphertext
{
    std::shared_ptr<SealCkksCiphertext> p;
    std::string str() const;

    SealCkksNativeCiphertext(); // new
    SealCkksNativeCiphertext(const SealCkksNativeCiphertext &); // = delete;
    SealCkksNativeCiphertext(const std::string & enc, cSealCkksNativeEvalKey ek);
};

} // e3
#endif // _E3_DEF_SEAL_CKKS_H_
// *** end cpp_share/util/def_seal_ckks.h

// *** begin cpp_share/util/def_seal_ckks0.h
#ifndef _E3_DEF_SEAL_CKKS0_H_
#define _E3_DEF_SEAL_CKKS0_H_

// this header def_seal_ckks0.h should not be included in any code except *seal_ckks0.cpp


namespace e3
{

struct SealCkksCiphertext { std::vector<double> ct; };

namespace e3seal_ckks
{
// struct SealCkksPrivKey
// {
// };

struct SealCkksEvalKey
{
    uint64_t polyModulusDegree = 0;
    uint64_t scale = 0;
};

// using sk = SealCkksPrivKey;
// inline  sk * tosk(e3::SealCkksNativePrivKey a) { return static_cast<sk *>(a); }

using ek = SealCkksEvalKey;
inline  ek * toek(e3::SealCkksNativeEvalKey a) { return static_cast<ek *>(a); }
inline  const ek * toek(e3::cSealCkksNativeEvalKey a) { return static_cast<const ek *>(a); }
} // e3seal_ckks
} // e3
#endif // _E3_DEF_SEAL_CKKS0_H_
// *** end cpp_share/util/def_seal_ckks0.h

// *** begin cpp_share/util/def_tfhe.h
#ifndef _E3_DEF_TFHE_H_
#define _E3_DEF_TFHE_H_


struct LweSample;

namespace e3
{

using TfheNativePrivKey = void * ;
using TfheNativeEvalKey = void * ;
using cTfheNativeEvalKey = const void * ;

struct TfheNativeBt
{
    std::shared_ptr<LweSample> p;
    std::string str(cTfheNativeEvalKey) const;
    TfheNativeBt(cTfheNativeEvalKey ek); // new
    TfheNativeBt(const std::string & enc, cTfheNativeEvalKey ek);
    TfheNativeBt(const TfheNativeBt &) = delete;
    TfheNativeBt(const TfheNativeBt & b, cTfheNativeEvalKey ek);
};

int tfhe_impl(); // 0 or 1

} // e3
#endif // _E3_DEF_TFHE_H_
// *** end cpp_share/util/def_tfhe.h

// *** begin cpp_share/util/def_tfhe0.h
#ifndef _E3_DEF_TFHE0_H_
#define _E3_DEF_TFHE0_H_

// this header def_tfhe0.h should not be included in any code except *tfhe0.cpp


struct LweSample { bool b; };

#endif // _E3_DEF_TFHE0_H_
// *** end cpp_share/util/def_tfhe0.h

// *** begin cpp_share/util/def_bfv.h
#ifndef _E3_DEF_BFV_H_
#define _E3_DEF_BFV_H_


namespace e3
{

class BfvProtEvalKey;

struct BfvProtNativeCiphertext
{
    std::string str() const;

    BfvProtNativeCiphertext(); // new
    BfvProtNativeCiphertext(const BfvProtNativeCiphertext &); // = delete;
    BfvProtNativeCiphertext(const std::string & enc, BfvProtEvalKey * ek);
};

} // e3

#endif // _E3_DEF_BFV_H_
// *** end cpp_share/util/def_bfv.h

// *** begin cpp_share/util/def_pali.h
#ifndef _E3_DEF_PALI_H_
#define _E3_DEF_PALI_H_



namespace e3
{

struct PaliBfvCiphertext;
class PaliBfvEvalKey;

struct PaliBfvNativeEvalKey
{
    void * cc;
    void * pk;
};

using PaliBfvNativePrivKey = void * ;
using cPaliBfvNativeEvalKey = const void * ;

struct PaliBfvNativeCiphertext
{
    std::shared_ptr<PaliBfvCiphertext> p;

    std::string str() const;

    PaliBfvNativeCiphertext(); // new
    PaliBfvNativeCiphertext(const PaliBfvNativeCiphertext &); // = delete;
    PaliBfvNativeCiphertext(const std::string & enc, const PaliBfvEvalKey * ek);

    using This = PaliBfvNativeCiphertext;
    This op_add(const This & a, void * cc) const;
    This op_sub(const This & a, void * cc) const;
    This op_mul(const This & a, void * cc) const;
};

int pali_impl(); // 0 or 1

} // e3
#endif // _E3_DEF_PALI_H_
// *** end cpp_share/util/def_pali.h

// *** begin cpp_share/util/def_pali0.h
#ifndef _E3_DEF_PALI0_H_
#define _E3_DEF_PALI0_H_



namespace e3
{

namespace pali
{
using vi64 = std::vector<int64_t>;
} // pali

} // e3

namespace e3
{

struct PaliBfvCiphertext
{
    std::vector<int64_t> x;
    std::string str() const;
    PaliBfvCiphertext operator+(const PaliBfvCiphertext & a) const;
    PaliBfvCiphertext operator-(const PaliBfvCiphertext & a) const;
    PaliBfvCiphertext operator*(const PaliBfvCiphertext & a) const;
};

namespace pali
{
void load_dummy();
} // pali

} // e3

#endif // _E3_DEF_PALI0_H_
// *** end cpp_share/util/def_pali0.h

// *** begin cpp_share/util/def_palisade_ckks.h
#ifndef _E3_DEF_PALISADE_CKKS_H_
#define _E3_DEF_PALISADE_CKKS_H_


namespace e3
{

struct PalisadeCkksCiphertext;
class PalisadeCkksEvalKey;

struct PalisadeCkksNativeEvalKey
{
    void * cc;
    void * pk;
};

using PalisadeCkksNativePrivKey = void * ;
using cPalisadeCkksNativeEvalKey = const void * ;

struct PalisadeCkksNativeCiphertext
{
    std::shared_ptr<PalisadeCkksCiphertext> p;

    std::string str() const;

    PalisadeCkksNativeCiphertext(); // new
    PalisadeCkksNativeCiphertext(const PalisadeCkksNativeCiphertext &); // = delete;
    PalisadeCkksNativeCiphertext(const std::string & enc, const PalisadeCkksEvalKey * ek);

    using This = PalisadeCkksNativeCiphertext;

    This add(const This & a, void * cc) const;
    This add(int a, void * cc) const;
    This add(double a, void * cc) const;
    This add(const std::complex<double> & a, void * cc) const;
    This add(const std::vector<int> & a, void * cc) const;
    This add(const std::vector<double> & a, void * cc) const;
    This add(const std::vector<std::complex<double>> & a, void * cc) const;

    This sub(const This & a, void * cc) const;
    This sub(int a, void * cc) const;
    This sub(double a, void * cc) const;
    This sub(const std::complex<double> & a, void * cc) const;
    This sub(const std::vector<int> & a, void * cc) const;
    This sub(const std::vector<double> & a, void * cc) const;
    This sub(const std::vector<std::complex<double>> & a, void * cc) const;

    This mul(const This & a, void * cc) const;
    This mul(int a, void * cc) const;
    This mul(double a, void * cc) const;
    This mul(const std::complex<double> & a, void * cc) const;
    This mul(const std::vector<int> & a, void * cc) const;
    This mul(const std::vector<double> & a, void * cc) const;
    This mul(const std::vector<std::complex<double>> & a, void * cc) const;

    This rot(int s, void * cc) const;
    std::vector<This> frot(const std::vector<int> & vs, void * cc) const;
};

// int pali_impl();

} // e3

#endif // _E3_DEF_PALISADE_CKKS_H_
// *** end cpp_share/util/def_palisade_ckks.h

// *** begin cpp_share/util/def_palisade_ckks0.h
#ifndef _E3_DEF_PALISADE_CKKS0_H_
#define _E3_DEF_PALISADE_CKKS0_H_


namespace e3
{

struct PalisadeCkksCiphertext
{
    std::vector<std::complex<double>> x;
    std::string str() const;
    PalisadeCkksCiphertext() {}
    PalisadeCkksCiphertext(const PalisadeCkksCiphertext & a)
        : PalisadeCkksCiphertext(a.x) {}
    PalisadeCkksCiphertext(int a, int n)
        : PalisadeCkksCiphertext( double(a), n ) {}
    PalisadeCkksCiphertext(double a, int n)
        : PalisadeCkksCiphertext( std::complex<double>(a), n ) {}
    PalisadeCkksCiphertext(const std::complex<double> & a, int n)
        : PalisadeCkksCiphertext( std::vector<std::complex<double>>(n >> 1, a) ) {}
    PalisadeCkksCiphertext(const std::vector<int> &);
    PalisadeCkksCiphertext(const std::vector<double> &);
    PalisadeCkksCiphertext(const std::vector<std::complex<double>> &);

    PalisadeCkksCiphertext operator+(const PalisadeCkksCiphertext & a) const;
    PalisadeCkksCiphertext operator-(const PalisadeCkksCiphertext & a) const;
    PalisadeCkksCiphertext operator*(const PalisadeCkksCiphertext & a) const;
    PalisadeCkksCiphertext rotate(int s) const;
};

namespace palisade_ckks
{

void dummy();

} // palisade_ckks

} // e3

#endif // _E3_DEF_PALISADE_CKKS0_H_
// *** end cpp_share/util/def_palisade_ckks0.h

// *** begin cpp_share/keys/anykey.h
#ifndef _E3_ANYKEY_H_
#define _E3_ANYKEY_H_


namespace e3
{

// There are 3 names used in the code
// 1. Decoration name (= name of the type) [KeyName::typ]
// 2. Encryption file name - name of the key [KeyName::fil]
// 3. Encryption type name - postfix for the Key class (~=cfg encryption) [clsenc]
struct KeyName
{
    std::string typ, fil;
};

struct AnyKey
{
    public: // need modify access from both Eval and Priv keys
        // it's possible to avoid public, but simplicity rules
        KeyName name;

        // utility function for decor
        static std::string prefix(
            const std::string & s, bool add, std::string pfx);

        AnyKey(KeyName nm) : name(nm) {}

        virtual std::string filename() const = 0;

        std::string decor(const std::string & what, bool add) const;
        virtual size_t slots() { return 1; }
};

} // e3
#endif // _E3_ANYKEY_H_
// *** end cpp_share/keys/anykey.h

// *** begin cpp_share/keys/ek_abstract.h
#ifndef _E3_EK_ABSTRACT_H_
#define _E3_EK_ABSTRACT_H_



namespace e3
{

class EvalKey : public AnyKey
{
    public:
        enum class User { Alice, Bob };

    private:
        void loadOrDie();

    protected:
        std::string filenamex(std::string fx) const
        { return name.fil + fx + ".eval.key"; }

        virtual std::string filename() const { return filenamex(""); }

        void user(User u) { if ( u == User::Bob ) loadOrDie(); }
        virtual bool load() = 0;

    public:
        EvalKey(KeyName nm) : AnyKey(nm) {}
        virtual std::string encrypt(const std::string & s, int msz) const;
};

} // e3

#endif // _E3_EK_ABSTRACT_H_
// *** end cpp_share/keys/ek_abstract.h

// *** begin cpp_share/keys/ek_seal.h
#ifndef _E3_EK_SEAL_H_
#define _E3_EK_SEAL_H_


namespace e3
{

class SealBaseEvalKey : public EvalKey
{
    private:
        static const bool NOCOUT = true;

    public:
        SealNativeEvalKey key;
        std::string rawEncrypt(const std::string & s, int msz) const;

    public:
        SealBaseEvalKey(int) : EvalKey({"", ""}), key() {} // when used as a member in key class
        SealBaseEvalKey(User u, KeyName name) : EvalKey(name), key() { user(u); }

        bool load(std::string fname);
        virtual bool load() { return load(filename()); }

        virtual std::string encrypt(const std::string & s, int msz) const
        { return decor(rawEncrypt(s, msz), true); }

        virtual size_t slots();
};

} // e3

#endif // _E3_EK_SEAL_H_
// *** end cpp_share/keys/ek_seal.h

// *** begin cpp_share/keys/ek_seal_ckks.h
#ifndef _E3_EK_SEAL_CKKS_H_
#define _E3_EK_SEAL_CKKS_H_


namespace e3
{

class SealCkksBaseEvalKey : public EvalKey
{
    private:
        static const bool NOCOUT = true;

    public:
        SealCkksNativeEvalKey key;
        std::string rawEncrypt(const std::string & s, int msz) const;

    public:
        SealCkksBaseEvalKey(int) : EvalKey({"", ""}), key() {}
        SealCkksBaseEvalKey(User u, KeyName name) : EvalKey(name), key() { user(u); }

        bool load(std::string fname);
        virtual bool load() { return load(filename()); }

        virtual std::string encrypt(const std::string & s, int msz) const
        { return decor(rawEncrypt(s, msz), true); }

        virtual size_t slots();
};

} // e3

#endif // _E3_EK_SEAL_CKKS_H_
// *** end cpp_share/keys/ek_seal_ckks.h

// *** begin cpp_share/keys/ek_arith_seal.h
#ifndef _E3_EK_ARITH_SEAL_H_
#define _E3_EK_ARITH_SEAL_H_


namespace e3
{

class SealEvalKey : public SealBaseEvalKey
{
    public:
        SealEvalKey(User u, KeyName name) : SealBaseEvalKey(u, name) {}
};

} // e3
#endif // _E3_EK_ARITH_SEAL_H_
// *** end cpp_share/keys/ek_arith_seal.h

// *** begin cpp_share/keys/ek_arith_seal_ckks.h
#ifndef _E3_EK_ARITH_SEAL_CKKS_H_
#define _E3_EK_ARITH_SEAL_CKKS_H_


namespace e3
{

class SealCkksEvalKey : public SealCkksBaseEvalKey
{
    public:
        SealCkksEvalKey(User u, KeyName name) : SealCkksBaseEvalKey(u, name) {}
};

} // e3
#endif // _E3_EK_ARITH_SEAL_CKKS_H_
// *** end cpp_share/keys/ek_arith_seal_ckks.h

// *** begin cpp_share/keys/ek_pil.h
#ifndef _E3_EK_PIL_H_
#define _E3_EK_PIL_H_


namespace e3
{

struct PilEvalValues
{
    const static int ver = 22;
    Bigun N;
    PilNum S;
    PilNum A, B, C, D, E, F;
};

struct PilArith;

class PilBaseEvalKey : public EvalKey
{
    public: // need for access from ekx classes
        PilEvalValues kv;

    public:
        PilBaseEvalKey(int) : EvalKey({"", ""}) {} // when used as a member in key class
        PilBaseEvalKey(User u, KeyName name) : EvalKey(name) { user(u); }

        bool load(string fname);
        virtual bool load() { return load(filename()); }

        friend struct PilArith;
};

class PilaEvalKey : public PilBaseEvalKey
{
    protected:

    public:
        PilaEvalKey(User u, KeyName name) : PilBaseEvalKey(u, name) {}
};

struct PilArith
{
    PilArith(const PilBaseEvalKey * ek): old(use) { use = &ek->kv; }
    ~PilArith() { use = old; }

    static const PilEvalValues * use;
    const PilEvalValues * old;

    // use safe or faster method
    static const PilEvalValues * u() { return use; }
    //static const PilEvalValues * u() { safe(); return use; }
    static void safe();
};


// arithmetic

inline PilNum operator+(PilNum a, PilNum b) { return PilNum(a.n.addmod(b.n, PilArith::u()->N)); }
inline PilNum operator-(PilNum a, PilNum b) { return PilNum(a.n.submod(b.n, PilArith::u()->N)); }
inline PilNum operator*(PilNum a, PilNum b) { return PilNum(a.n.mulmod(b.n, PilArith::u()->N)); }
inline PilNum operator%(PilNum a, PilNum b) { return PilNum(a.n % b.n); }

inline bool operator==(PilNum a, PilNum b) { return a.n == b.n; }
inline bool operator!=(PilNum a, PilNum b) { return !(a == b); }

inline PilPair operator*(PilPair x, PilNum n) { return {x.a * n, x.b * n}; }
inline PilPair operator*(PilNum n, PilPair x) { return x * n; }
inline PilPair operator+(PilPair x, PilPair y) { return {x.a + y.a, x.b + y.b}; }
inline PilPair operator-(PilPair x, PilPair y) { return {x.a - y.a, x.b - y.b}; }
PilPair operator*(PilPair x, PilPair y);

inline PilQuad operator-(PilQuad a, PilQuad b) { return {a.x - b.x, a.y - b.y}; }
inline PilQuad operator+(PilQuad a, PilQuad b) { return {a.x + b.x, a.y + b.y}; }
PilQuad operator*(PilQuad a, PilQuad b);


} // e3
#endif // _E3_EK_PIL_H_
// *** end cpp_share/keys/ek_pil.h

// *** begin cpp_share/keys/ek_pail.h
#ifndef _E3_EK_PAIL_H_
#define _E3_EK_PAIL_H_


namespace e3
{

class PailEvalKey : public EvalKey
{
    protected:
        struct Processor
        {
            Bigun N, N2, B2, A2;
            int beta = 0, high_bit_posN = 0, high_bit_posN2 = 0;
            Bigun Xp1, Xp2;

            void setB2Beta(int beta);
            void init(Bigun n);
            string show() const;

            bool leq_x(const e3::Bigun & x) const { return (x < Xp1 || Xp2 < x); }
        };

        Processor proc;

    public:
        void setN(Bigun p, Bigun q) { setN(p * q); }
        void setN(Bigun aN) { proc.init(aN); }

        friend class PailEvalKeyExt;

    public:
        PailEvalKey(int) : EvalKey({"", ""}) {} // when used as a member in key class
        PailEvalKey(User u, KeyName name) : EvalKey(name) { user(u); }

        bool load(string fname);
        virtual bool load() { return load(filename()); }

        const Bigun & getN() const { return proc.N; }
        const Bigun & getN2() const { return proc.N2; }
        int getBeta() const { return proc.beta; }
        const Bigun & getB2() const { return proc.B2; }

        string show() const { return proc.show(); }
        void setBeta(int b) { proc.setB2Beta(b); }
        bool leq(Bigun a) { return proc.leq_x(a); }
};

} // e3

#endif // _E3_EK_PAIL_H_
// *** end cpp_share/keys/ek_pail.h

// *** begin cpp_share/keys/ek_circ.h
#ifndef _E3_EK_CIRC_H_
#define _E3_EK_CIRC_H_



namespace e3
{

class CircuitEvalKey : public EvalKey
{
    public:
        CircuitEvalKey(KeyName name) : EvalKey(name) { }

        static std::vector<std::string> enc2bits(const std::string & e);
        static std::string bits2enc(const std::vector<std::string> & v);
};

} // e3
#endif // _E3_EK_CIRC_H_
// *** end cpp_share/keys/ek_circ.h

// *** begin cpp_share/keys/ek_circ_bdd.h
#ifndef _E3_EK_CIRC_BDD_H_
#define _E3_EK_CIRC_BDD_H_


namespace e3
{

class CircuitEvalKey_bdd : public CircuitEvalKey
{
        static const bool NOCOUT = true;

    public:
        using Bnek = std::string;

    protected:
        Bnek key;

    public:
        CircuitEvalKey_bdd(User u, KeyName name)
            : CircuitEvalKey(name) { user(u); }

        virtual bool load();

        virtual std::string filename() const;
};

class CircuitEvalKey_bddn : public CircuitEvalKey_bdd
{
    public:
        CircuitEvalKey_bddn(User u, KeyName name)
            : CircuitEvalKey_bdd(u, name) {}
};

class CircuitEvalKey_bdda : public CircuitEvalKey_bdd
{
    public:
        CircuitEvalKey_bdda(User u, KeyName name)
            : CircuitEvalKey_bdd(u, name) {}
};

class CircuitEvalKey_bddf : public CircuitEvalKey_bdd
{
    public:
        CircuitEvalKey_bddf(User u, KeyName name)
            : CircuitEvalKey_bdd(u, name) {}
};

} // e3
#endif // _E3_EK_CIRC_BDD_H_
// *** end cpp_share/keys/ek_circ_bdd.h

// *** begin cpp_share/keys/ek_circ_fhew.h
#ifndef _E3_EK_CIRC_FHEW_H_
#define _E3_EK_CIRC_FHEW_H_


namespace e3
{

class CircuitEvalKey_fhew : public CircuitEvalKey
{
        static const bool NOCOUT = false;

    public:
        using Fnek = FhewNativeEvalKey;

    protected:
        Fnek key;

    public:
        CircuitEvalKey_fhew(User u, KeyName name)
            : CircuitEvalKey(name), key() { user(u); }

        bool load_oleg();
        bool load_fhew();
        virtual bool load();

        cFhewNativeEvalKey native() const { return key; }

        virtual std::string filename() const;
};

} // e3
#endif // _E3_EK_CIRC_FHEW_H_
// *** end cpp_share/keys/ek_circ_fhew.h

// *** begin cpp_share/keys/ek_circ_heli.h
#ifndef _E3_EK_CIRC_HELI_H_
#define _E3_EK_CIRC_HELI_H_


namespace e3
{

class CircuitEvalKey_heli : public CircuitEvalKey
{
        static const bool NOCOUT = true;

    public:
        using Hnek = HeliNativeEvalKey;
        NativeProperties * properties = nullptr;

    protected:
        Hnek key;

    public:
        CircuitEvalKey_heli(User u, KeyName name)
            : CircuitEvalKey(name), key() { user(u); }

        virtual bool load();

        cHeliNativeEvalKey native() const { return key; }

        virtual std::string filename() const;
};

} // e3
#endif // _E3_EK_CIRC_HELI_H_
// *** end cpp_share/keys/ek_circ_heli.h

// *** begin cpp_share/keys/ek_circ_pil.h
#ifndef _E3_EK_CIRC_PIL_H_
#define _E3_EK_CIRC_PIL_H_


namespace e3
{

class CircuitEvalKey_pilc : public CircuitEvalKey
{
        static const bool NOCOUT = false;

    public: // let nameBit directly access
        PilBaseEvalKey bek;

    public:
        CircuitEvalKey_pilc(User u, KeyName name)
            : CircuitEvalKey(name), bek(0) { user(u); }

        virtual bool load() { return bek.load(filename()); }
        virtual std::string filename() const { return EvalKey::filename(); }

    private:
        void dummy() const;
};


} // e3
#endif // _E3_EK_CIRC_PIL_H_
// *** end cpp_share/keys/ek_circ_pil.h

// *** begin cpp_share/keys/ek_circ_plain.h
#ifndef _E3_EK_CIRC_PLAIN_H_
#define _E3_EK_CIRC_PLAIN_H_


namespace e3
{

class CircuitEvalKey_plain : public CircuitEvalKey
{
    protected:
        e3::util::usi key;

    public:
        CircuitEvalKey_plain(User u, KeyName name)
            : CircuitEvalKey(name), key() { user(u); }

        virtual bool load();

        static bool dec(e3::util::usi x, e3::util::usi k) { return !!(x - k); }
        static e3::util::usi enc(bool x, e3::util::usi k) { return (x ? 1 : 0) + k; }

        bool dec(e3::util::usi x) const { return dec(x, key); }
        e3::util::usi enc(bool x) const { return enc(x, key); }
};

} // e3
#endif // _E3_EK_CIRC_PLAIN_H_
// *** end cpp_share/keys/ek_circ_plain.h

// *** begin cpp_share/keys/ek_circ_seal.h
#ifndef _E3_EK_CIRC_SEAL_H_
#define _E3_EK_CIRC_SEAL_H_


using std::string;

namespace e3
{

class CircuitEvalKey_seal_bfv : public CircuitEvalKey
{
        static const bool NOCOUT = true;

    protected:
        SealBaseEvalKey bek;

    public:
        CircuitEvalKey_seal_bfv(User u, KeyName name)
            : CircuitEvalKey(name), bek(0) { user(u); }

        virtual bool load() { return bek.load(filename()); }

        cSealNativeEvalKey native() const { return bek.key; }

        virtual string encbitstr(bool b) const
        {
            return bek.rawEncrypt(std::to_string(unsigned(b)), 1);
        }

        virtual std::string encrypt(const std::string & s, int msz) const
        {
            std::vector<string> numbers = e3::util::split(s, '_');
            std::vector<std::vector<bool>> bin(msz);
            for ( size_t i = 0; i < numbers.size(); i++ )
            {
                auto hexvalue = e3::util::dec2hex(numbers[i], msz);
                auto sbit = e3::util::hex2bin(hexvalue, msz);
                for ( size_t j = 0; j < sbit.size(); j++ )
                    bin[j].push_back( sbit[j] );
            }
            std::vector<string> bits;
            for ( auto b : bin )
                bits.push_back( bek.rawEncrypt(e3::util::merge(b, "_"), msz) );

            return decor( CircuitEvalKey::bits2enc(bits), true );
        }

        virtual string filename() const { return EvalKey::filename(); }
        virtual size_t slots() { return bek.slots(); }
};

} // e3
#endif // _E3_EK_CIRC_SEAL_H_
// *** end cpp_share/keys/ek_circ_seal.h

// *** begin cpp_share/keys/ek_circ_tfhe.h
#ifndef _E3_EK_CIRC_TFHE_H_
#define _E3_EK_CIRC_TFHE_H_


namespace e3
{

class CircuitEvalKey_tfhe : public CircuitEvalKey
{
        static const bool NOCOUT = true;

    public:
        using Tnek = TfheNativeEvalKey;

    protected:
        Tnek key;

    public:
        CircuitEvalKey_tfhe(User u, KeyName name)
            : CircuitEvalKey(name), key() { user(u); }

        virtual bool load();

        cTfheNativeEvalKey native() const { return key; }

        virtual std::string filename() const;
};

} // e3
#endif // _E3_EK_CIRC_TFHE_H_
// *** end cpp_share/keys/ek_circ_tfhe.h

// *** begin cpp_share/keys/ek_native.h
#ifndef _E3_EK_NATIVE_H_
#define _E3_EK_NATIVE_H_


namespace e3
{

class NativeEvalKey : public EvalKey
{
    protected:
        e3::util::ull key;

    public:
        NativeEvalKey(User u, KeyName name)
            : EvalKey(name), key() { user(u); } // user must be called here

        virtual bool load(); // load has to be public so PrivKey can call it.
        // Another solution would be SK calling the construction of EK

        e3::util::ull dec(e3::util::ull x) const { return dec(x, key); }
        e3::util::ull enc(e3::util::ull x) const { return enc(x, key); }
        static e3::util::ull dec(e3::util::ull x, e3::util::ull k) { return x ^ k; }
        static e3::util::ull enc(e3::util::ull x, e3::util::ull k) { return x ^ k; }
};

} // e3
#endif // _E3_EK_NATIVE_H_
// *** end cpp_share/keys/ek_native.h

// *** begin cpp_share/keys/ek_bfv_prot.h
#ifndef _E3_EK_BFVPROT_H_
#define _E3_EK_BFVPROT_H_


namespace e3
{

class BfvProtEvalKey : public EvalKey
{
    private:
        static const bool NOCOUT = false;

    public: // access from PrivKey
        int nslots = 1;

        uint64_t polyModulusDegree = 1 << 1;
        uint64_t plainModulus = 2;

    public:
        std::string rawEncrypt(const std::string & s, int msz) const;

    public:
        BfvProtEvalKey(int) : EvalKey({"", ""}) {} // when used as a member in key class
        BfvProtEvalKey(User u, KeyName name) : EvalKey(name) { user(u); }

        bool load(std::string fname);
        virtual bool load() { return load(filename()); }

        virtual std::string encrypt(const std::string & s, int msz) const
        { return decor(rawEncrypt(s, msz), true); }

        virtual size_t slots();
};

} // e3

#endif // _E3_EK_BFVPROT_H_
// *** end cpp_share/keys/ek_bfv_prot.h

// *** begin cpp_share/keys/ek_pali.h
#ifndef _E3_EK_PALI_H_
#define _E3_EK_PALI_H_


namespace e3
{

struct PaliBfvQuery
{
    virtual void print() const = 0;
};

class PaliBfvEvalKey : public EvalKey
{
    private:
        static const bool NOCOUT = true;

    public:
        PaliBfvNativeEvalKey key;

        int polyDegree = 65537;
        int mulDepth = 2;
        int useSlots = 1;
        int maxDepth = 2;
        int p_n = 8 * 1024;

        std::string rawEncrypt(const std::string & s, int msz) const;

    public:
        PaliBfvEvalKey(int) : EvalKey({"", ""}), key() {}
        PaliBfvEvalKey(User u, KeyName name) : EvalKey(name), key() { user(u); }

        bool load(std::string fname);
        virtual bool load() { return load(filename()); }

        virtual std::string encrypt(const std::string & s, int msz) const
        { return decor(rawEncrypt(s, msz), true); }

        virtual size_t slots() const;

        PaliBfvQuery * query() const;
};

} // e3

#endif // _E3_EK_PALI_H_
// *** end cpp_share/keys/ek_pali.h

// *** begin cpp_share/keys/ek_circ_gatcou.h
#ifndef _E3_EK_CIRC_GATCOU_H_
#define _E3_EK_CIRC_GATCOU_H_


namespace e3
{

class CircuitEvalKey_gatcou : public CircuitEvalKey
{
        static const bool NOCOUT = false;

    public: // let nameBit directly access

    public:
        CircuitEvalKey_gatcou(User u, KeyName name)
            : CircuitEvalKey(name) { user(u); }

        virtual bool load() { return true; }
        virtual std::string filename() const { return EvalKey::filename(); }

    private:
        void dummy() const;
};

using CircuitEvalKey_gatcow = CircuitEvalKey_gatcou;

} // e3
#endif // _E3_EK_CIRC_GATCOU_H_
// *** end cpp_share/keys/ek_circ_gatcou.h

// *** begin cpp_share/keys/ek_palisade_ckks.h
#ifndef _E3_EK_PALISADE_CKKS_H_
#define _E3_EK_PALISADE_CKKS_H_


namespace e3
{

class PalisadeCkksEvalKey : public EvalKey
{
    private:
        static const bool NOCOUT = true;

    public:
        PalisadeCkksNativeEvalKey key;

        uint32_t multDepth = 1;
        uint32_t scaleFactorBits = 50;
        uint32_t batchSize = 1;
        int securityLevel = 128;
        int ringDimension = 0;
        std::string rescale = "exact";
        std::string rotations = "none";

        std::string rawEncrypt(const std::string & strPlaintext, int msz) const;

    public:
        PalisadeCkksEvalKey(int) : EvalKey( {"", ""} ), key() {}
        PalisadeCkksEvalKey(User u, KeyName name) : EvalKey(name), key() { user(u); }

        bool load(std::string fname);
        virtual bool load() { return load( filename() ); }

        virtual std::string encrypt(const std::string & strPlaintext, int msz) const
        { return decor(rawEncrypt(strPlaintext, msz), true); }

        virtual size_t slots() const;
};

} // e3

#endif // _E3_EK_PALISADE_CKKS_H_
// *** end cpp_share/keys/ek_palisade_ckks.h

// *** begin cpp_share/util/e3math.inc


template <class T>
void e3::math::Meuclid<T>::checkRec(
    const Rec & v0, const Rec & v,
    const Rec & vb, T et, T ft)
{
    T x = add( mul(v0.a, v.e) , mul(v0.b, v.f) );
    T y = add( mul(vb.a, et) , mul(vb.b, ft) );

    if ( x != y )
        throw "Error: [pej+qfj!=aet+bft] " + x.str() + ":" + y.str();
}


template <class T>
void e3::math::Meuclid<T>::fillAB(T x, T y)
{
    if (x < y) x.swap(y);
    v.clear();
    Rec r;
    r.a = x;
    r.b = y;
    v.push_back(r);

    size_t i = 0;
    if ( v[i].b != 0 )
        while ( 1 )
        {
            //v[i].c = v[i].a%v[i].b;
            //v[i].d = v[i].a/v[i].b;
            T::divABRQ(v[i].a, v[i].b, &v[i].c, &v[i].d);
            if ( v[i].c == 0 ) break;
            v.push_back(Rec()); i++;

            v[i].a = v[i - 1].b;
            v[i].b = v[i - 1].c;
        }
}

template <class T>
void e3::math::Meuclid<T>::fillEF()
{
    int n = int(v.size()) - 1;

    for ( int i = n; i >= 0; i-- )
    {
        v[i].e = et;
        v[i].f = ft;
        for ( int j = i - 1; j >= 0; j-- )
        {
            v[j].e = v[size_t(j) + 1].f;
            v[j].f = sub( v[size_t(j) + 1].e, mul( v[j].d, v[j].e ) );
        }

        v[i].e = v[0].e;
        v[i].f = v[0].f;
    }
}

template <class T>
string e3::math::Meuclid<T>::str() const
{
    string r;
    for ( int i = 0; i < (int)v.size(); i++ )
    {
        r += v[i].a.str() + ' ' + v[i].b.str() + ' ' + v[i].c.str()
             + ' ' + v[i].d.str() + ' ' + v[i].e.str() + ' '
             + v[i].f.str() + ' ' + v[i].g.str() + '\n';
    }
    return r;
}

template <class T>
void e3::math::Meuclid<T>::fillG()
{
    int n = int(v.size()) - 1;

    for ( int i = n; i >= 0; i-- )
    {
        checkRec(v[0], v[i], v[i], et, ft);

        v[i].g = add( add( mul(v[i].a, et), mul(v[i].b, ft) ), mul(v[i].e, v[i].f) );

        if (0)if ( v[i].g < 0 )
            {
                T y = add( v[i].e, v[0].b );
                if ( y > 0 )
                {
                    //T x = (-v[i].g)/y + 1;
                    T x = add( divq( (neg(v[i].g)), y) , 1 );
                    //v[i].f += x;
                    v[i].f.addMod(x, mod);
                    v[i].g = add( add( mul(v[0].a, v[i].e),
                                       mul(v[0].b, v[i].f) ), mul(v[i].e, v[i].f) );
                }
            }

        if ( v[i].g > 0 )
        {
            if ( gsmall == 0  || gsmall > v[i].g ) gsmall = v[i].g;
        }
    }
}

template<typename T> bool e3::math::invert(const T & x, const T & mod, T * xm1)
{
    using UC = UnumberContractor<T>;

    UC ucx(x);
    UC ucmod(mod);

    Invertor<UC> iuc(ucx, ucmod);

    if ( !iuc.isOk() ) return false;

    if (xm1) *xm1 = iuc.get().x;

    return true;
}

// *** end cpp_share/util/e3math.inc

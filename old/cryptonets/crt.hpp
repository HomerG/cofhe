#pragma once

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <string>
#include <vector>
#include "seal/seal.h"

using namespace seal;
using std::string;
using std::vector;

template <class T, class U> T
multiply_by_ull(const T & x, U u, const T zero)
{
    if ( u == 0 ) return zero;
    if ( u == 1 ) return x;
    if ( u < 0 ) return -multiply_by_ull(x, -u, zero);

    T y = x + x;
    auto q = u / 2;
    y = multiply_by_ull(y, q, zero);

    if ( u == 2 * q ) return y;
    return y + x;
}

template <class Number>
class CRT
{
    private:
        vector<Plaintext> vp;
        vector<Ciphertext> vc;
        bool encrypted;
        static vector<Number> coprimes;
        static Number mod;
        static vector<Number> n;
        static vector<Number> x;
        static vector<Ciphertext> zero;
        static vector<RelinKeys> relinkeys;
        static vector<BatchEncoder*> encoders;
        static vector<Decryptor*> decryptors;
        static vector<Encryptor*> encryptors;
        static vector<Evaluator*> evaluators;

    public:
        CRT() : CRT( Number(0) ) {}
        // CRT(int dummy, const vector<uint64_t> & v, bool encrypted);
        CRT(const vector<vector<uint64_t>> &, bool encrypted);
        CRT(const CRT & a);
        CRT(const Number & a, bool encrypted=false) { *this = encode(a, encrypted); }
        CRT(const vector<Number> & a, bool encrypted=false) { *this = encode(a, encrypted); }
        CRT(const vector<Ciphertext> &);

        CRT & operator+=(const CRT & a);
        CRT & operator*=(const CRT & a);
        CRT & operator*=(Number a) { return *this = *this * a; }

        CRT operator-() const;
        CRT operator+(const CRT & a) const { CRT r = *this; r += a; return r; }
        CRT operator*(const CRT & a) const { CRT r = *this; r *= a; return r; }
        CRT operator*(Number a) const { return multiply_by_ull(*this, a, CRT(zero) ); }
        friend CRT operator+(Number a, const CRT & b) { return b+a; }
        static vector<Number> decode(const CRT & a, bool is_signed=true);
        static CRT encode(const Number & a, bool encrypted=false);
        static CRT encode(const vector<Number> & a, bool encrypted=false);
        vector<Number> decode(bool is_signed=true) const { return CRT::decode(*this, is_signed); }
        string str() const;

        bool isEncrypted() const { return encrypted; }

        static size_t slots() { return encoders[0]->slot_count(); }

        string noise() const {
            string noise = "";
            for ( size_t i=0; i<coprimes.size(); i++ )
                noise += std::to_string( decryptors[i]->invariant_noise_budget(vc[i]) ) + " ";
            return noise.substr(0,noise.size()-1);
        }

        static vector<Number> getCoprimes() { return coprimes; }
        static void setCoprimes(const vector<Number> & coprimes);
        static void setParams(
            const vector<RelinKeys> & relinkeys,
            const vector<BatchEncoder*> & encoders,
            const vector<Decryptor*> &  decryptors,
            const vector<Encryptor*> & encryptors,
            const vector<Evaluator*> & evaluators
        );
};

template <class Number> vector<Number> CRT<Number>::coprimes;
template <class Number> Number CRT<Number>::mod;
template <class Number> vector<Number> CRT<Number>::n;
template <class Number> vector<Number> CRT<Number>::x;
template <class Number> vector<Ciphertext> CRT<Number>::zero;
template <class Number> vector<RelinKeys> CRT<Number>::relinkeys;
template <class Number> vector<BatchEncoder*> CRT<Number>::encoders;
template <class Number> vector<Decryptor*> CRT<Number>::decryptors;
template <class Number> vector<Encryptor*> CRT<Number>::encryptors;
template <class Number> vector<Evaluator*> CRT<Number>::evaluators;

template <class Number>
CRT<Number>::CRT(const CRT<Number> & a)
{
    encrypted = a.encrypted;
    if ( encrypted )
        for ( auto c : a.vc )
        {
            // vc.push_back(c);
            Ciphertext nc = c;
            vc.push_back(nc);
        }
    else
        for ( auto p : a.vp ) vp.push_back(p);
}

// template <class Number>
// CRT<Number>::CRT(const vector<uint64_t> & v, bool encrypted)
// {
//     this->encrypted = encrypted;
//     for ( size_t i=0; i<coprimes.size(); i++ )
//     {
//         vector<uint64_t> int_p ( encoders[i]->slot_count(), v[i] );
//         Plaintext p;
//         encoders[i]->encode(int_p, p);
//         if (encrypted)
//         {
//             Ciphertext c;
//             encryptors[i]->encrypt(p, c);
//             vc.push_back(c);
//         }
//         else vp.push_back(p);
//     }
// }

template <class Number>
CRT<Number>::CRT(const vector<vector<uint64_t>> & v, bool encrypted)
{
    this->encrypted = encrypted;
    for ( size_t i=0; i<coprimes.size(); i++ )
    {
        Plaintext p;
        encoders[i]->encode(v[i], p);
        if (encrypted)
        {
            Ciphertext c;
            encryptors[i]->encrypt(p, c);
            vc.push_back(c);
        }
        else vp.push_back(p);
    }
}

template <class Number>
CRT<Number>::CRT(const vector<Ciphertext> & v)
{
    this->encrypted = true;
    for ( size_t i=0; i<coprimes.size(); i++ )
    {
        Ciphertext c = v[i];
        vc.push_back(c);
    }
}

template <class Number>
CRT<Number> & CRT<Number>::operator+=(const CRT & a)
{
    if ( encrypted )
    {
        for ( size_t i=0; i<coprimes.size(); i++ )
        {
            if ( a.encrypted ) evaluators[i]->add_inplace(vc[i], a.vc[i]);
            else evaluators[i]->add_plain_inplace(vc[i], a.vp[i]);
        }
    }
    else throw "Unencrypted addition not implemented";
    return *this;
}

template <class Number>
CRT<Number> & CRT<Number>::operator*=(const CRT & a)
{
    if ( encrypted )
    {
        for ( size_t i=0; i<coprimes.size(); i++ )
        {
            if ( a.encrypted ) evaluators[i]->multiply_inplace(vc[i], a.vc[i]);
            else evaluators[i]->multiply_plain_inplace(vc[i], a.vp[i]);
            evaluators[i]->relinearize_inplace(vc[i], relinkeys[i]);
        }
    }
    else throw "Unencrypted multiplication not implemented";
    return *this;
}

template <class Number>
CRT<Number> CRT<Number>::operator-() const
{
    CRT r = *this;
    if ( r.encrypted )
    {
        for ( size_t i=0; i<coprimes.size(); i++ )
            evaluators[i]->sub(zero[i], r.vc[i], r.vc[i]);
            // evaluators[i]->negate(r.vc[i], r.vc[i]);
    }
    else throw "Unencrypted unary minus not implemented";
    return r;
}

template <class Number>
vector<Number> CRT<Number>::decode(const CRT<Number> & a, bool is_signed)
{
    auto size = coprimes.size();

    // Number & nf = mod;
    // vector<Number> n ( size, 1 );
    // for ( size_t i=0; i<size; i++ ) n[i] = nf / coprimes[i];
    //
    // vector<Number> x;
    // for ( size_t i=0; i<size; i++ )
    // {
    //     Number xi = 0;
    //     while ( xi * n[i] % coprimes[i] != 1 ) xi++;
    //     x.push_back(xi);
    // }

    vector<Plaintext> vp;
    if ( a.encrypted )
    {
        for ( size_t i=0; i<size; i++ )
        {
            Plaintext p;
            decryptors[i]->decrypt(a.vc[i], p);
            vp.push_back(p);
        }
    }
    else vp = a.vp;

    vector<vector<Number>> m;
    for ( size_t i=0; i<size; i++ )
    {
        vector<uint64_t> decoded;
        encoders[i]->decode(vp[i], decoded);
        vector<Number> v;
        for ( auto e : decoded ) v.push_back( Number(e) );
        m.push_back(v);
    }

    auto nElems = m[0].size();
    vector<vector<Number>> vf( nElems, vector<Number>(size) );
    for ( size_t i=0; i<nElems; i++ )
        for ( size_t j=0; j<size; j++ ) vf[i][j] = m[j][i] * n[j] * x[j] % mod;

    vector<Number> vr;
    for ( auto f : vf )
    {
        Number r = 0;
        for ( auto & e : f ) r += e;
        r %= mod;
        r = (is_signed && r>mod/2) ? r-mod : r;
        vr.push_back(r);
    }
    return vr;
}

template <class Number>
CRT<Number> CRT<Number>::encode(const Number & a, bool encrypted)
{
    return encode( vector<Number>(encoders[0]->slot_count(), a), encrypted );
    // auto n = a % mod;
    // n = n<0 ? mod+n : n;
    // vector<uint64_t> v;
    // for ( auto e : coprimes ) v.push_back( uint64_t(n % e) );
    // return CRT<Number>(0, v, encrypted);
}

template <class Number>
CRT<Number> CRT<Number>::encode(const vector<Number> & a, bool encrypted)
{
    auto size = encoders[0]->slot_count();
    vector<Number> n(size);
    for ( size_t i=0; i<size; i++ )
    {
        n[i] = a[i] % mod;
        n[i] = n[i]<0 ? mod+n[i] : n[i];
    }
    vector<vector<uint64_t>> m;
    for ( auto e : coprimes )
    {
        vector<uint64_t> v;
        for ( size_t i=0; i<size; i++ ) v.push_back( uint64_t(n[i] % e) );
        m.push_back(v);
    }
    return CRT<Number>(m, encrypted);
}

template <class Number>
string CRT<Number>::str() const
{
    vector<Plaintext> decrypted;
    if (encrypted)
    {
        for ( size_t i=0; i<coprimes.size(); i++ )
        {
            Plaintext p;
            decryptors[i]->decrypt(vc[i], p);
            decrypted.push_back(p);
        }
    }
    else decrypted = this->vp;

    vector<uint64_t> v;
    for ( size_t i=0; i<coprimes.size(); i++ )
    {
        vector<uint64_t> decoded;
        encoders[i]->decode(decrypted[i], decoded);
        v.push_back( decoded[0] );
    }

    string s = "";
    for ( auto & e : v ) s += std::to_string(e) + " ";
    return s.substr(0, s.size()-1);
}

template <class Number> void
CRT<Number>::setCoprimes(const vector<Number> & coprimes)
{
    CRT<Number>::coprimes.clear();
    std::copy( coprimes.begin(), coprimes.end(), std::back_inserter(CRT<Number>::coprimes) );
    mod = 1;
    for ( auto & e : coprimes ) mod *= e;

    // Number & nf = mod;
    // vector<Number> n ( size, 1 );
    n.clear();
    // for ( size_t i=0; i<size; i++ ) n[i] = nf / coprimes[i];
    for ( auto coprime : coprimes ) n.push_back( mod / coprime );

    // vector<Number> x;
    x.clear();
    for ( size_t i=0; i<coprimes.size(); i++ )
    {
        Number xi = 0;
        while ( xi * n[i] % coprimes[i] != 1 ) xi++;
        x.push_back(xi);
    }
}

template <class Number> void
CRT<Number>::setParams(
    const vector<RelinKeys> & relinkeys,
    const vector<BatchEncoder*> & encoders,
    const vector<Decryptor*> &  decryptors,
    const vector<Encryptor*> & encryptors,
    const vector<Evaluator*> & evaluators
)
{
    CRT<Number>::relinkeys = relinkeys;
    CRT<Number>::encoders = encoders;
    CRT<Number>::decryptors = decryptors;
    CRT<Number>::encryptors = encryptors;
    CRT<Number>::evaluators = evaluators;

    zero.clear();
    auto slots = encoders[0]->slot_count();
    vector<uint64_t> z(slots, 0);
    for ( size_t i=0; i<encryptors.size(); i++ )
    {
        Plaintext p;
        encoders[i]->encode(z, p);
        Ciphertext c;
        encryptors[i]->encrypt(p, c);
        zero.push_back(c);
    }
}

template <class Number>
inline std::ostream & operator<<(std::ostream & os, const CRT<Number> & a)
{
    return os << a.str();
}

#include <iostream>
#include <vector>
#include "seal/seal.h"
#include "crt.hpp"
#include "io.hpp"
#include "ml.hpp"
#include "numpy.hpp"

using namespace seal;
using namespace std;

typedef __int128 Number;

int main(int argc, char * argv[])
try {
    // --- SEAL --- //
    if (argc < 2)
    {
        cout << "Inform polynomial modulus degree\n";
        return 1;
    }

    size_t poly_modulus_degree = 1 << stoull(argv[1]);
    cout << "Polynomial modulus degree: " << poly_modulus_degree << "\n";

    // const vector<Number> coprimes{40961, 65537, 114689, 147457, 188417};
    // const vector<Number> coprimes{65537, 114689, 147457, 163841};
    const vector<Number> coprimes{65537, 114689, 147457, 163841, 557057};

    vector<RelinKeys> relinkeys;
    vector<BatchEncoder*> encoders;
    vector<Encryptor*> encryptors;
    vector<Evaluator*> evaluators;
    vector<Decryptor*> decryptors;
    for ( auto & plaintext_modulus : coprimes )
    {
        EncryptionParameters p(scheme_type::BFV);
        p.set_poly_modulus_degree(poly_modulus_degree);
        p.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
        p.set_plain_modulus(plaintext_modulus);
        auto context = SEALContext::Create(p);
        auto qualifiers = context->first_context_data()->qualifiers();
        cout << "Batching enabled: " << boolalpha << qualifiers.using_batching << endl;
        KeyGenerator keygen(context);
        PublicKey public_key = keygen.public_key();
        SecretKey secret_key = keygen.secret_key();
        relinkeys.push_back( keygen.relin_keys() );
        encryptors.push_back( new Encryptor(context, public_key) );
        decryptors.push_back( new Decryptor(context, secret_key) );
        evaluators.push_back( new Evaluator(context) );
        encoders.push_back( new BatchEncoder(context) );
    }

    // --- DATA LOADING AND ENCODING --- //
    CRT<Number>::setCoprimes(coprimes);
    CRT<Number>::setParams(relinkeys, encoders, decryptors, encryptors, evaluators);

    cout << "Start!\n";

    vector<Number> v;
    size_t size = 1024;
    Number half = size >> 1;
    for ( size_t i=0; i<size; i++ ) v.push_back( i-half );

    CRT<Number> a(5, true);//, b(-3, false);
    Number b = -3;
    cout << "a " << a << " | " << int64_t( a.decode()[0] ) << '\n';
    // cout << "b " << b << " | " << int64_t( b.decode()[0] ) << '\n';
    cout << "b " << int64_t(b) << '\n';
    cout << "add\n";
    auto add = a+b;
    cout << "mul\n";
    auto mul = a*b;
    cout << "sq\n";
    auto sq  = mul*mul;
    cout << "add " << add << " | " << int64_t( add.decode()[0] ) << '\n';
    cout << "mul " << mul << " | " << int64_t( mul.decode()[0] ) << '\n';
    cout << "sq  " << sq  << " | " << int64_t(  sq.decode()[0] ) << '\n';

    cout << "Done!\n";
} catch (const char * str)
{
    std::cout << "Exception: " << str << std::endl;
}

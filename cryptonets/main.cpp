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
        cout << "Inform polynomial degree\n";
        return 1;
    }

    size_t poly_modulus_degree = 1 << stoull(argv[1]);
    cout << "Polynomial degree: " << poly_modulus_degree << "\n";

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
	auto & context_data = *context->key_context_data();
	auto coeff_modulus = context_data.parms().coeff_modulus();
	cout << "-----------------------------------------------\n";
	cout << "Plaintext modulus: " << int64_t(plaintext_modulus) << "\n";
        cout << "Batching enabled: " << boolalpha << qualifiers.using_batching << "\n";
	cout << "Coeff modulus size: " << context_data.total_coeff_modulus_bit_count() << " bits ( ";
	for ( auto & c : coeff_modulus )
		cout << c.bit_count() << " ";
	cout << ")\n";
        KeyGenerator keygen(context);
        PublicKey public_key = keygen.public_key();
        SecretKey secret_key = keygen.secret_key();
        relinkeys.push_back( keygen.relin_keys() );
        encryptors.push_back( new Encryptor(context, public_key) );
        decryptors.push_back( new Decryptor(context, secret_key) );
        evaluators.push_back( new Evaluator(context) );
        encoders.push_back( new BatchEncoder(context) );
    }
    cout << "-----------------------------------------------\n";

    // --- DATA LOADING AND ENCODING --- //
    CRT<Number>::setCoprimes(coprimes);
    CRT<Number>::setParams(relinkeys, encoders, decryptors, encryptors, evaluators);

    const bool subset = true;
    size_t interval_begin = 0, interval_end = poly_modulus_degree;
    const size_t sbit = 6;
    const size_t ibit = sbit, w1bit = sbit, w4bit = sbit, w8bit = sbit;
    const size_t b1bit = ibit + w1bit;
    const size_t b4bit = 2*b1bit + w4bit + 2;
    const size_t b8bit = 2*b4bit + w8bit + 4;
    const Number scaler_i  = 1 << ibit;
    const Number scaler_w1 = 1 << w1bit;
    const Number scaler_w4 = 1 << w4bit;
    const Number scaler_w8 = 1 << w8bit;
    const Number scaler_b1 = shift( Number(1), size_t(b1bit) );
    const Number scaler_b4 = shift( Number(1), size_t(b4bit) );
    const Number scaler_b8 = shift( Number(1), size_t(b8bit) );
    cout << "Precision/scale:\n";
    cout << " input: " << ibit << '\n';
    cout << " weights: " << w1bit << " " << w4bit << " " << w8bit << '\n';
    cout << " biases: " << b1bit << " " << b4bit << " " << b8bit << '\n';
    cout << '\n';

    cout << "load .. " << flush;
    auto x_test_0 = load<double>("x_test.tmp", ',');
    auto x_test_double = reshape4d(x_test_0, vector<size_t>{x_test_0.size(),28,28,1} );
    auto y_test_size_t = load<size_t>("y_test.tmp");
    auto w1_double = reshape4d( load<double>("W1.tmp", ','), vector<size_t>{5,5,1,5} );
    auto b1_double = load<double>("b1.tmp");
    auto w4_double = reshape4d( load<double>("W4.tmp", ','), vector<size_t>{5,5,5,10} );
    auto b4_double = load<double>("b4.tmp");
    auto w8_double = load<double>("W8.tmp", ',');
    auto b8_double = load<double>("b8.tmp");
    cout << "ok\n";

    vector<vector<vector<vector<double>>>> x_test_tmp;
    // vector<double> y_test_tmp;
    vector<size_t> y_test_tmp;
    if ( subset )
    {
        for ( size_t i=interval_begin; i<interval_end; i++ )
        {
            x_test_tmp.push_back( x_test_double[i] );
            y_test_tmp.push_back( y_test_size_t[i] );
        }
    }
    else
    {
        x_test_tmp = x_test_double;
        y_test_tmp = y_test_size_t;
    }

    cout << "scale .. " << flush;
    auto x_test_scaled = scale(x_test_tmp, scaler_i);
    // auto y_test = scale(y_test_tmp, size_t(1));
    auto & y_test = y_test_tmp;
    auto w1_scaled = scale(w1_double, scaler_w1);
    auto w4_scaled = scale(w4_double, scaler_w4);
    auto w8_scaled = scale(w8_double, scaler_w8);
    auto b1_scaled = scale(b1_double, scaler_b1);
    auto b4_scaled = scale(b4_double, scaler_b4);
    auto b8_scaled = scale(b8_double, scaler_b8);
    cout << "ok\n";

    cout << "encode/encrypt .. " << flush;
    auto x_test = reshapeOrder( crtEncryptPack( reshapeOrder(x_test_scaled, vector<size_t>{1,2,3,0}) ), vector<size_t>{3,0,1,2} );
    auto & w1 = w1_scaled;
    auto & w4 = w4_scaled;
    auto & w8 = w8_scaled;
    auto b1 = crtEncode(b1_scaled);
    auto b4 = crtEncode(b4_scaled);
    auto b8 = crtEncode(b8_scaled);
    cout << "ok\n";

    cout << "\nDimensions:\n";
    cout << "x_test_0 "; print(shape(x_test_0));
    cout << "x_test_reshaped "; print(shape(x_test_double));
    cout << "x_test_tmp "; print(shape(x_test_tmp));
    cout << "x_test "; print(shape(x_test));
    cout << "y_test "; print(shape(y_test));
    cout << "y_test_tmp "; print(shape(x_test_tmp));
    cout << "w1 "; print(shape(w1));
    cout << "b1 "; print(shape(b1));
    cout << "w4 "; print(shape(w4));
    cout << "b4 "; print(shape(b4));
    cout << "w8 "; print(shape(w8));
    cout << "b8 "; print(shape(b8));

    cout << "\nPredict\n";
    double acc;
    auto y_test_hat = predict(x_test,y_test,w1,b1,w4,b4,w8,b8,acc);
    cout << "The accuracy on test data for MNIST: " << acc << '\n';
    cout << "Done!\n";
} catch (const char * str)
{
    std::cout << "Exception: " << str << std::endl;
}

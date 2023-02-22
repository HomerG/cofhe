#include <cassert>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "functions.hpp"
#include "ringint.hpp"

using namespace std;
using Base = Number;
using Type = Ringint;

int main(int argc, char * argv[])
try
{
    int nTries   = argc > 1 ? stoi( argv[1] ) : 1;
    int maxValue = argc > 2 ? stoi( argv[2] ) : 100;
    Type::setModulus(maxValue);

    enum Constructor { CDEFAULT, CINT, CNUMBER, CSTR, CTYPE };
    const vector<string> constructors = {
        "CDEFAULT", "CINT    ", "CNUMBER ", "CTYPE   ", "CSTR    "
    };
    enum Operator {
        ADD, SUB, MUL, DIV, MOD,
        GCD, INV, LCM, POW,
         EQ,  GE,  GT,  LE,  LT,  NE
    };
    const vector<string> operators = {
        "ADD", "SUB", "MUL", "DIV", "MOD",
        "GCD", "INV", "LCM", "POW",
        "EQ ", "GE ", "GT ", "LE ", "LT ", "NE "
    };

    cout << '\n';
    while (nTries--)
    {
        cout << "Testing constructors\n";
        for ( int i=0; i<constructors.size(); i++ )
        {
            uint64_t ia = rand() % maxValue;
            Base ir;
            Type cr;
            switch (i)
            {
                case CDEFAULT : ir = Base(); cr = Type();       break;
                case CINT     : ir = ia;     cr = ia;           break;
                case CNUMBER  : ir = ia;     cr = ir;           break;
                case CSTR     : ir = ia;     cr = string(ir);   break;
                case CTYPE    : ir = ia;     cr = Type(ir);     break;
                default : throw "Test for constructor not implemented";
            }
            checkResult(constructors[i], cr, ir);
        }
        cout << '\n';

        cout << "Testing operators\n";
        Base im = maxValue;
        Base ia = rand() % im;
        Base ib = rand() % im;
        while (!ib || ia < ib)
        {
            ia = rand() % im;
            ib = rand() % im;
        }
        cout << "Operands: " << ia << ", " << ib << '\n';
        for ( int i=0; i<operators.size(); i++ )
        {

            Base ir;
            Type cr, ca = ia, cb = ib;
            switch (i)
            {
                case ADD : ir = (ia + ib) % im; cr = ca + cb; break;
                case SUB :
                     ir = (ia + (ia < ib ? im: Base(0) ) - ib ) % im;
                     cr = ca - cb; break;
                case MUL : ir = (ia * ib) % im; cr = ca * cb; break;
                case DIV :
                {
                    bool ierror = false, cerror = false;
                    try { ir = ia * inv(ib,im) % im; }
                    catch(const char * e) { ierror = true; }
                    try { cr = ca / cb; }
                    catch(const char * e) { cerror = true; }
                    if ( ierror && cerror ) cr = ir = 0;
                    break;
                }
                case MOD : ir = (ia % ib) % im; cr = ca % cb; break;
                case GCD : ir = Base::gcd(ia, ib) % im; cr = Type::gcd(ca, cb); break;
                case INV :
                {
                    bool ierror = false, cerror = false;
                    try { ir = inv(ia, im); }
                    catch(const char * e) { ierror = true; }
                    try { cr = Type::inv(ca); }
                    catch(const char * e) { cerror = true; }
                    if ( ierror && cerror ) cr = ir = 0;
                    break;
                }
                case LCM : ir = Base::lcm(ia, ib); cr = Type::lcm(ca, cb); break;
                case POW : ir = Base::pow(ia, ib, im); cr = Type::pow(ca, cb); break;
                case  EQ : ir = ia == ib; cr = ca == cb; break;
                case  GE : ir = ia >= ib; cr = ca >= cb; break;
                case  GT : ir = ia >  ib; cr = ca >  cb; break;
                case  LE : ir = ia <= ib; cr = ca <= cb; break;
                case  LT : ir = ia <  ib; cr = ca <  cb; break;
                case  NE : ir = ia != ib; cr = ca != cb; break;
                default : throw "Test for operator not implemented";
            }
            checkResult(operators[i], cr, ir);
        }
        cout << '\n';
    }
}
catch (const char   * e) { cout << e << '\n'; }
catch (const string & e) { cout << e << '\n'; }

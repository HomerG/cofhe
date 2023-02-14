#include <cassert>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "functions.hpp"
#include "number.h"

using namespace std;
using Type = Number;

int main(int argc, char * argv[])
try
{
    int nTries   = argc > 1 ? stoi( argv[1] ) : 1;
    int maxValue = argc > 2 ? stoi( argv[2] ) : 100;

    enum Constructor { CDEFAULT, CINT, CCHAR, CSTR, CTYPE };
    const vector<string> constructors = {
        "CDEFAULT", "CINT    ", "CCHAR   ", "CTYPE   ", "CSTR    "
    };
    enum Operator {
        ADD, SUB, MUL, DIV, MOD, SLL, SRL,
        AND,  OR, XOR,
        GCD, INV, LCM, POW,
         EQ,  GE,  GT,  LE,  LT,  NE
    };
    const vector<string> operators = {
        "ADD", "SUB", "MUL", "DIV", "MOD", "SLL", "SRL",
        "AND", "OR ", "XOR",
        "GCD", "INV", "LCM", "POW",
        "EQ ", "GE ", "GT ", "LE ", "LT ", "NE "
    };

    cout << '\n';
    while (nTries--)
    {
        cout << "Testing constructors\n";
        for ( int i=0; i<constructors.size(); i++ )
        {
            int  ir;
            Type cr;
            switch (i)
            {
                case CDEFAULT : ir = 0; cr = Type(); break;
                case CINT  : ir = rand(); cr = ir; break;
                case CCHAR : ir = rand(); cr = to_string(ir).c_str(); break;
                case CSTR  : ir = rand(); cr = to_string(ir); break;
                case CTYPE : ir = rand(); cr = Type(ir); break;
                default : throw "Test for constructor not implemented";
            }
            checkResult(constructors[i], cr, ir);
        }
        cout << '\n';

        cout << "Testing operators\n";
        uint64_t ia = rand() % maxValue;
        uint64_t ib = rand() % maxValue;
        uint64_t ic = rand() % 4; // shift constant and exponent
        while (!ib || ia < ib)
        {
            ia = rand() % maxValue;
            ib = rand() % maxValue;
        }
        cout << "Operands: " << ia << ", " << ib << ", " << ic << '\n';
        for ( int i=0; i<operators.size(); i++ )
        {

            int  ir;
            Type cr, ca = ia, cb = ib, cc = ic;
            switch (i)
            {
                case ADD : ir = ia + ib; cr = ca + cb; break;
                case SUB : ir = ia - ib; cr = ca - cb; break;
                case MUL : ir = ia * ib; cr = ca * cb; break;
                case DIV : ir = ia / ib; cr = ca / cb; break;
                case MOD : ir = ia % ib; cr = ca % cb; break;
                case SLL : ir = ia >>ic; cr = ca >>cc; break;
                case SRL : ir = ia <<ic; cr = ca <<cc; break;
                case AND : ir = ia & ib; cr = ca & cb; break;
                case  OR : ir = ia | ib; cr = ca | cb; break;
                case XOR : ir = ia ^ ib; cr = ca ^ cb; break;
                case GCD : ir = gcd(ia, ib); cr = Type::gcd(ca, cb); break;
                case INV :
                {
                    bool ierror = false, cerror = false;
                    try { ir = inv(ia, ib); }
                    catch(const char * e) { ierror = true; }
                    try { cr = Type::inv(ca, cb); }
                    catch(const char * e) { cerror = true; }
                    if ( ierror && cerror ) cr = ir = 0;
                    break;
                }
                case LCM : ir = lcm(ia, ib); cr = Type::lcm(ca, cb); break;
                case POW : ir = pow(ia, ic, ib); cr = Type::pow(ca, cc, cb); break;
                case  EQ : ir = ia ==ib; cr = ca ==cb; break;
                case  GE : ir = ia >=ib; cr = ca >=cb; break;
                case  GT : ir = ia > ib; cr = ca > cb; break;
                case  LE : ir = ia <=ib; cr = ca <=cb; break;
                case  LT : ir = ia < ib; cr = ca < cb; break;
                case  NE : ir = ia !=ib; cr = ca !=cb; break;
                default : throw "Test for operator not implemented";
            }
            checkResult(operators[i], cr, ir);
        }
        cout << '\n';
    }
}
catch (const char   * e) { cout << e << '\n'; }
catch (const string & e) { cout << e << '\n'; }

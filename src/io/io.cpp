#include "io.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace io
{

vector<vector<uint64_t>> load(const string & filename)
{
    vector<vector<uint64_t>> data;
    ifstream fin(filename);
    for( string line; getline(fin, line); )
    {
        vector<uint64_t> v;
        stringstream ss(line);
        for ( string item; ss >> item; ) v.push_back( uint64_t( stoull(item) ) );
        data.push_back(v);
    }
    return data;
}

void print(const uint8_t * din, size_t size, size_t divider)
{
    cout << hex;
    for ( size_t i=0; i<size; )
    {
        cout << uint16_t( din[i++] );
        cout << ( divider && ( i % divider ) ? ' ' : '\n' );
    }
    cout << dec;
}

void printPair(const Number & n, const uint8_t * u, size_t size)
{
    printPair(n.str(16), u, size);
}

void printPair(const string & n, const uint8_t * u, size_t size)
{
    cout << n;
    for (int i=0; i<size; i++) cout << hex << ' ' << uint32_t(u[i]);
    cout << '\n';
}

void printPair(const vector<Number> & vn, const uint8_t * u, size_t size)
{
    for (int i=0; i<vn.size(); i++)
        printPair(vn[i], &u[i*size], size);
}

void printPair(const vector<string> & vn, const uint8_t * u, size_t size)
{
    for (int i=0; i<vn.size(); i++)
        printPair(vn[i], &u[i*size], size);
}

void printShort(const vector<Ringint> & v)
{
    if (v.size() >= 4)
        cout << v[0] << ", " << v[1] << ", ... "
            << v[v.size()-2] << ", " << v.back() << '\n';
    else for (auto & e : v)
    {
        cout << e << ' ';
        cout << '\n';
    }
}

vector<string> read(const string & filename)
{
    vector<string> vout;
    ifstream fin(filename);
    string line;
    while ( getline(fin, line) ) vout.push_back(line);
    return vout;
}

} // io

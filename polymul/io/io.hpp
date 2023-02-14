#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace io
{

template <class T>
void print(const std::vector<T> & v)
{
    std::cout << "{ ";
    for ( auto & e : v ) std::cout << e << ' ';
    std::cout << "}\n";
}

} // io

#pragma once

#ifdef EDUINT

#include "integer.hpp"
using Number = Integer;

#else

#include "bigint.hpp"
using Number = Bigint;

#endif


#ifndef never
#define nevers(x) (throw std::string("err at ")+(__FILE__)+":"+std::to_string(__LINE__) + " ["+x+"]")
#define never (throw std::string("err at ")+(__FILE__)+":"+std::to_string(__LINE__) + " ["+(__func__)+"]")
#endif

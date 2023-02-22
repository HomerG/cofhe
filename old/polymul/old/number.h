#pragma once

#ifdef EDUINT

#include "integer.hpp"
using Number = Integer;

#else

#include "e3int.h"
#include "e3key.h"

using Number = e3::Bigun;

#endif


#ifndef never
#define nevers(x) (throw std::string("err at ")+(__FILE__)+":"+std::to_string(__LINE__) + " ["+x+"]")
#define never (throw std::string("err at ")+(__FILE__)+":"+std::to_string(__LINE__) + " ["+(__func__)+"]")
#endif

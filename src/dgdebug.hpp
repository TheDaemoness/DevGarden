#ifndef DGDEBUG_HPP
#define DGDEBUG_HPP

//For those of us who can't use GDB.

#include <iostream>
#include <cassert>

#undef NDEBUG

#define DEBUG_EMIT(a) std::cerr << a << std::endl;
#define DEBUG_EMIT_TEST(a,b) if(a) {std::cerr << b << std::endl;}
#define DEBUG_ASSERT(a) assert(a);
#define DEBUG_ASSERT_TEST(a,b) if(a) assert(b);

#endif // DGDEBUG_HPP

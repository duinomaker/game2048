#ifndef __SIMPLE_TEST__
#define __SIMPLE_TEST__
#include <cassert>
#include <iostream>
#include <string>

#define INIT_TEST(_name)             \
    std::string _test_name = #_name; \
    std::string _test_type;          \
    int main(int, char**)            \
    {

#define TEST(_type) _test_type = #_type;

#define EXPECT_EQ(_a, _b) assert((_a) == (_b));
#define EXPECT_TRUE(_a) assert(_a);
#define EXPECT_FALSE(_a) assert(!(_a));

#define ENDTEST() std::clog << "[ " << _test_name << " ] " << _test_type << " Accepted" << std::endl;

#define TERMINATE_TEST()                                                          \
    std::clog << "[ " << _test_name << " ] All tests are accepted." << std::endl; \
    }

#endif // __SIMPLE_TEST__